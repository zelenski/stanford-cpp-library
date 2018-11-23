package stanford.cs106.util;

import java.io.*;
import java.util.*;
import javax.xml.*;
import javax.xml.parsers.*;
import javax.xml.stream.*;
import javax.xml.transform.dom.*;
import javax.xml.validation.*;
import org.w3c.dom.*;
import org.xml.sax.*;

public class XmlUtils {
    /** Represents exceptions that can occur while processing XML text. */
    public static class XmlRuntimeException extends RuntimeException {
        private static final long serialVersionUID = 0;
        
        public XmlRuntimeException(String message) {
            super(message);
        }
        
        public XmlRuntimeException(String message, Throwable cause) {
            super(message, cause);
        }
    }
    
    public static final String XML_EXTENSION = ".xml";
    
    public static boolean attributeIsFalsey(Node node, String name) {
        return hasAttribute(node, name) && StringUtils.isFalsey(getAttribute(node, name));
    }
    
    public static boolean attributeIsTruthy(Node node, String name) {
        return hasAttribute(node, name) && StringUtils.isTruthy(getAttribute(node, name));
    }
    
    public static void ensureNotNull(Object node, String tag) {
        if (node == null) {
            throw new XmlRuntimeException("missing required node or attribute: <" + tag + ">");
        }
    }
    
    /**
     * Returns the value of the attribute of the given node with the given name.
     * @param node the XML node to examine
     * @param name the attribute name for which to retrieve the value
     * @return the attribute value, or null if the attribute does not exist for this node
     */
    public static String getAttribute(Node node, String name) {
        if (node.getAttributes() == null) {
            return null;
        }
        Node attributeNode = node.getAttributes().getNamedItem(name);
        if (attributeNode == null) {
            return null;
        } else {
            return attributeNode.getNodeValue();
        }
    }
    
    public static void setAttribute(Node node, String name, String value) {
        Element element = (Element) node;
        element.setAttribute(name, value);
    }
    
    public static String getAttribute(Node node, String name, boolean required) {
        return getAttribute(node, name, required, null);
    }
    
    public static String getAttribute(Node node, String name, boolean required, String defaultValue) {
        if (required && !hasAttribute(node, name)) {
            throw new XmlRuntimeException("missing required attribute: " + name);
        }
        String attributeValue = getAttribute(node, name);
        if (attributeValue == null) {
            attributeValue = defaultValue;
        }
        return attributeValue;
    }

    public static int getAttributeInt(Node node, String name) {
        return getAttributeInt(node, name, false);
    }
    
    public static int getAttributeInt(Node node, String name, boolean required) {
        return getAttributeInt(node, name, required, 0);
    }

    public static int getAttributeInt(Node node, String name, boolean required, int defaultValue) {
        if (required && !hasAttribute(node, name)) {
            throw new XmlRuntimeException("missing required attribute: " + name);
        }
        String attributeValue = getAttribute(node, name);
        if (attributeValue != null) {
            try {
                return Integer.parseInt(attributeValue);
            } catch (NumberFormatException nfe) {
            	// empty
            }
        }
        return defaultValue;
    }
    
    /**
     * Returns the value of the first attribute found with any of the given names, else null.
     * @param node the XML node to examine
     * @return the attribute value, or null if the attribute does not exist for this node
     */
    public static String getAttributeAny(Node node, String... names) {
        return getAttributeAny(node, false, names);
    }
    
    /**
     * Returns the value of the first attribute found with any of the given names, else null.
     * @param node the XML node to examine
     * @return the attribute value, or null if the attribute does not exist for this node
     */
    public static String getAttributeAny(Node node, boolean required, String... names) {
        if (node.getAttributes() != null) {
            for (String name : names) {
                Node attributeNode = node.getAttributes().getNamedItem(name);
                if (attributeNode != null) {
                    return attributeNode.getNodeValue();
                }
            }
        }
        
        if (required) {
        	throw new XmlRuntimeException("missing required attributes (one or more must be present): " + Arrays.toString(names));
        } else {
        	return null;
        }
    }
    
    public static String getAttributeOrChild(Node node, String name) {
        return getAttributeOrChild(node, name, false);
    }

        /**
     * Returns the value of the attribute of the given node with the given name.
     * @param node the XML node to examine
     * @param name the attribute name for which to retrieve the value
     * @return the attribute value, or null if the attribute does not exist for this node
     */
    public static String getAttributeOrChild(Node node, String name, boolean required) {
        if (hasAttribute(node, name)) {
            return getAttribute(node, name);
        } else {
            return getChildValue(node, name, required);
        }
    }
    
    /**
     * Returns all attributes of the given XML node as a list of name=value strings.
     * Mostly just for debugging.
     * @param node
     * @return
     */
    public static List<String> getAttributes(Node node) {
        List<String> result = new ArrayList<String>();
        NamedNodeMap attrs = node.getAttributes();
        for (int i = 0; i < attrs.getLength(); i++) {
            Node attrNode = attrs.item(i);
            result.add(attrNode.getNodeName() + "=" + attrNode.getNodeValue());
        }
        return result;
    }

    public static Node getChildNode(Node node, String element) {
        List<Node> childNodes = getChildNodes(node, element);
        if (childNodes.isEmpty()) {
            return null;
        } else {
            return childNodes.get(0);
        }
    }
    
    public static List<Node> getChildNodes(Node node, String... elements) {
        List<Node> result = new ArrayList<Node>();
        NodeList childNodes = node.getChildNodes();
        for (int i = 0; i < childNodes.getLength(); i++) {
            Node childNode = childNodes.item(i);
            for (String element : elements) {
                if (element.equals(childNode.getNodeName())) {
                    result.add(childNode);
                }
            }
        }
        return result;
    }
    
    public static String getChildValue(Node node, String element) {
        return getChildValue(node, element, false);
    }
    
    public static String getChildValue(Node node, String element, boolean required) {
        String result = null;
        NodeList childNodes = node.getChildNodes();
        for (int i = 0; i < childNodes.getLength(); i++) {
            Node childNode = childNodes.item(i);
            if (childNode.getNodeName().equals(element)) {
                result = getTextContent(childNode, true);
                break;
            }
        }
        
        if (required) {
            ensureNotNull(result, element);
        }
        
        return result;
    }
    
    public static Node getChildByTagName(Node node, String element, boolean required) {
        Node result = getChildByTagName(node, element);
        if (required) {
            ensureNotNull(result, element);
        }
        return result;
    }

    public static Node getChildByTagName(Node node, String element) {
        NodeList childNodes = node.getChildNodes();
        for (int j = 0; j < childNodes.getLength(); j++) {
            Node childNode = childNodes.item(j);
            if ("*".equals(element) || element == null || childNode.getNodeName().equals(element)) {
                return childNode;
            }
        }
        return null;
    }
    
    public static List<Node> getChildrenByTagName(Node node, String element) {
        List<Node> result = new ArrayList<Node>();
        NodeList childNodes = node.getChildNodes();
        for (int j = 0; j < childNodes.getLength(); j++) {
            Node childNode = childNodes.item(j);
            if ("*".equals(element) || element == null || childNode.getNodeName().equals(element)) {
                result.add(childNode);
            }
        }
        return result;
    }
    
    public static List<Node> getChildrenByTagNames(Node node, String... elements) {
        List<Node> result = new ArrayList<Node>();
        NodeList childNodes = node.getChildNodes();
        for (int j = 0; j < childNodes.getLength(); j++) {
            Node childNode = childNodes.item(j);
            for (String element : elements) {
                if ("*".equals(element) || element == null || childNode.getNodeName().equals(element)) {
                    result.add(childNode);
                }
            }
        }
        return result;
    }
    
    public static Element getElementByTagName(Document document, String element) {
        NodeList nodeList = document.getElementsByTagName(element);
        if (nodeList.getLength() > 0) {
            return (Element) nodeList.item(0);
        } else {
            return null;
        }
    }
    
    public static String getTextContent(Node node) {
        return getTextContent(node, true);
    }
    
    public static String getTextContent(Node node, boolean trim) {
        if (node == null) {
            return "";
        }
        
        String textContent = "";
        NodeList childNodes = node.getChildNodes();
        for (int i = 0; i < childNodes.getLength(); i++) {
            Node childNode = childNodes.item(i);
            if (childNode.getNodeType() == Node.TEXT_NODE) {
                textContent += childNode.getNodeValue().trim();
            }
        }
        
//        String textContent = node.getTextContent();
//        if (textContent == null) {
//            return "";
//        }
        
        textContent = textContent.replace("\r", "");
        if (textContent.startsWith("\n")) {
            textContent = textContent.substring(1);
        }
        
        if (trim) {
            textContent = textContent.trim();
        }
        
        return textContent;
    }
    
    /**
     * Returns whether the given XML DOM node contains the given attribute.
     * @param node the XML node to examine
     * @param name an attribute name to check
     * @return true if the attribute exists, false if not
     */
    public static boolean hasAttribute(Node node, String name) {
        return node.getAttributes().getNamedItem(name) != null;
    }
    
    /**
     * Returns the value of the first attribute found with any of the given names, else null.
     * @param node the XML node to examine
     * @return the attribute value, or null if the attribute does not exist for this node
     */
    public static boolean hasAttributeAny(Node node, String... names) {
        for (String name : names) {
            if (hasAttribute(node, name)) {
                return true;
            }
        }
        return false;
    }
    
    /**
     * Returns whether the given XML DOM node contains all of the given attributes.
     * @param node the XML node to examine
     * @param names a variable-length list of attribute names to check
     * @return true if all attributes exist, false if not
     */
    public static boolean hasAttributes(Node node, String... names) {
        NamedNodeMap attrs = node.getAttributes();
        for (String name: names) {
            if (attrs.getNamedItem(name) == null) {
                return false;
            }
        }
        
        return true;
    }

    /**
     * Ensures that the given XML DOM node contains all of the given attributes.
     * Throws an exception if any attribute is not found.
     * @param node the XML node to examine
     * @param names a variable-length list of attribute names to check
     * @throws XMLStreamException if any attribute is not found
     */
    public static void requireAttributes(Node node, String... names) {
        NamedNodeMap attrs = node.getAttributes();
        for (String name: names) {
            if (attrs.getNamedItem(name) == null) {
                throw new XmlRuntimeException("<" + node.getNodeName() + "> element requires \"" + name + "\" attribute");
            }
        }
    }
    
    public static void validate(String xmlFileName, String schemaFileName) throws IOException, ParserConfigurationException, SAXException {
        SchemaFactory schemaFactory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
        Schema schema = schemaFactory.newSchema(new File(schemaFileName));
        Validator validator = schema.newValidator();
        
        DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
        builderFactory.setNamespaceAware(true);
        DocumentBuilder parser =  builderFactory.newDocumentBuilder();
        Document document = parser.parse(new File(xmlFileName));
        validator.validate(new DOMSource(document));
    }
    
    public static Node up(Node node, String element) {
        while (node != null && node.getNodeName() != null && !node.getNodeName().equalsIgnoreCase(element)) {
            node = node.getParentNode();
        }
        return node;
    }
}
