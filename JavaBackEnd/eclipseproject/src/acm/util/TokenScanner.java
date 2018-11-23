package acm.util;

import java.io.*;
import java.util.ArrayList;
import java.util.Stack;

// Referenced classes of package acm.util:
//            ErrorException

public class TokenScanner
{

    public TokenScanner()
    {
        ignoreWhitespaceFlag = false;
        ignoreCommentsFlag = false;
        scanNumbersFlag = false;
        scanStringsFlag = false;
        cpos = 0;
        input = null;
        wordChars = "";
        savedTokens = new Stack<String>();
        operators = new ArrayList<String>();
    }

    public TokenScanner(String s)
    {
        this();
        setInput(s);
    }

    public TokenScanner(Reader reader)
    {
        this();
        setInput(reader);
    }

    public void setInput(String s)
    {
        setInput(((Reader) (new StringReader(s))));
    }

    public void setInput(Reader reader)
    {
        savedChars = "";
        savedTokens.clear();
        cpos = 0;
        input = reader;
    }

    public boolean hasMoreTokens()
    {
        String s = nextToken();
        saveToken(s);
        return !s.isEmpty();
    }

    public String nextToken()
    {
        if(!savedTokens.isEmpty())
            return (String)savedTokens.pop();
        else
            return scanToken();
    }

    public void saveToken(String s)
    {
        savedTokens.push(s);
    }

    public int getPosition()
    {
        if(savedTokens.isEmpty())
            return cpos;
        else
            return cpos - ((String)savedTokens.peek()).length();
    }

    public void ignoreWhitespace()
    {
        ignoreWhitespaceFlag = true;
    }

    public void ignoreComments()
    {
        ignoreCommentsFlag = true;
    }

    public void scanNumbers()
    {
        scanNumbersFlag = true;
    }

    public void scanStrings()
    {
        scanStringsFlag = true;
    }

    public void addWordCharacters(String s)
    {
        for (int i = 0; i < s.length(); i++) {
        	char ch = s.charAt(i);
        	if (wordChars.indexOf(ch) == -1) {
        		wordChars += ch;
        	}
        }
    }

    public boolean isWordCharacter(int i)
    {
        if(i == -1)
            return false;
        else
            return Character.isLetterOrDigit(i) || wordChars.indexOf((char)i) >= 0;
    }

    public void addOperator(String s)
    {
        operators.add(s);
    }

    public void verifyToken(String s)
    {
        String s1 = nextToken();
        if(s1.equals(s))
        {
            return;
        } else
        {
            String s2 = (new StringBuilder()).append("Found ").append(s1).append(" when expecting ").append(s).toString();
            throw new ErrorException(s2);
        }
    }

    public int getTokenType(String s)
    {
        if(s == null || s.length() == 0)
            return -1;
        char c = s.charAt(0);
        if(Character.isWhitespace(c))
            return 0;
        if(Character.isDigit(c))
            return 2;
        if(c == '"' || c == '\'')
            return 3;
        return !isWordCharacter(c) ? 4 : 1;
    }

    /* Reverse-engineered from the following bytecode by Keith (htiek@cs.stanford.edu). Please let me
     * know if I messed this up!
     * 
     * public int getChar();
    Code:
       0: aload_0
       1: dup
       2: getfield      #6                  // Field cpos:I
       5: iconst_1
       6: iadd
       7: putfield      #6                  // Field cpos:I
      10: aload_0
      11: getfield      #21                 // Field savedChars:Ljava/lang/String;
      14: invokevirtual #25                 // Method java/lang/String.isEmpty:()Z
      17: ifne          43
      20: aload_0
      21: getfield      #21                 // Field savedChars:Ljava/lang/String;
      24: iconst_0
      25: invokevirtual #33                 // Method java/lang/String.charAt:(I)C
      28: istore_1
      29: aload_0
      30: aload_0
      31: getfield      #21                 // Field savedChars:Ljava/lang/String;
      34: iconst_1
      35: invokevirtual #50                 // Method java/lang/String.substring:(I)Ljava/lang/String;
      38: putfield      #21                 // Field savedChars:Ljava/lang/String;
      41: iload_1
      42: ireturn
      43: aload_0
      44: getfield      #7                  // Field input:Ljava/io/Reader;
      47: invokevirtual #51                 // Method java/io/Reader.read:()I
      50: ireturn
      51: astore_1
      52: new           #45                 // class acm/util/ErrorException
      55: dup
      56: aload_1
      57: invokespecial #53                 // Method acm/util/ErrorException."<init>":(Ljava/lang/Exception;)V
      60: athrow
    Exception table:
       from    to  target type
           0    42    51   Class java/io/IOException
          43    50    51   Class java/io/IOException
     * 
     */
    public int getChar()
    {
    	try
    	{
    		cpos++;
    		if (!savedChars.isEmpty())
    		{
    			char result = savedChars.charAt(0);
    			savedChars = savedChars.substring(1);
    			return result;
    		}

    		return input.read();
    	}
    	catch (IOException e)
    	{
    		throw new ErrorException(e);
    	}
    }  

    public void ungetChar(int i)
    {
        cpos--;
        if(input instanceof PushbackReader)
            try
            {
                ((PushbackReader)input).unread(i);
                return;
            }
            catch(IOException ioexception) { }
        if(i >= 0)
            savedChars = (new StringBuilder()).append((char)i).append(savedChars).toString();
    }

    /* Given a string containing some escape sequences, returns a new string formed by resolving all of
     * those escape sequences. The understood escapes are
     * 
     *    \a  \b  \f  \n   \r   \t   \v   \"   \'   \\   \xHH  \OOO
     * 
     * TODO: Given the context in which this is being used, it may make sense
     * to have this assume the encoding format is UTF-8 and always decode the
     * string assuming that encoding. Otherwise, we have to postprocess the
     * string to fix the UTF-8 weirdness.
     */
    public String getStringValue(String s)
    {
        String result = "";
        boolean isQuoted = false;
       
        /* Track where we want to stop. If the string begins with a quote mark of some
         * sort, the assumption is that it also ends with one, so we'll stop one character
         * before then.
         */
        int payloadEnd = s.length();
        if(payloadEnd > 1 && (s.charAt(0) == '"' || s.charAt(0) == '\''))
        {
            isQuoted = true;
            payloadEnd--;
        }
        
        /* Visit each character in sequence, processing escape sequences. */
        for(int i = (isQuoted ? 1 : 0); i < payloadEnd; i++)
        {
            char c = s.charAt(i);
            
            /* Start of an escape sequence. */
            if(c == '\\')
            {
            	/* Look at the next character. In the course of doing so, advance the read pointer
            	 * forward so that we don't revisit this character later on.
            	 */
                c = s.charAt(++i);
                
                /* We could be reading an octal (\OOO) or hexadecimal (\xHH) literal. If so, we need
                 * to decode the digits to determine what the intended byte value is.
                 */
                if(Character.isDigit(c) || c == 'x')
                {
                	byte base;
                	int  charsInNumber;
                	
                	/* Hexadecimal. */
                	if (c == 'x')
                	{
                		base = 16;
                		charsInNumber = 2;
                		
                		/* Skip the 'x' itself. The payload is right after it. */
                		i++;
                	}
                	/* Octal. */
                	else
                	{
                		base = 8;
                		charsInNumber = 3;
                	}
                	
                    
                    int charValue  = 0;
                    
                    /* Scan forward until we hit the end of the payload or have read all the digits we want.
                     * The "stop early" condition is how we handle things like \0 appearing at the end of a
                     * number.
                     */
                    for(int rangeEnd = Math.min(payloadEnd, i + charsInNumber); i < rangeEnd; i++)
                    {
                        c = s.charAt(i);
                        int thisCharsValue;
                        
                        /* Determine the numeric value of this character. */
                        if(Character.isDigit(c))
                        {
                            thisCharsValue = c - '0';
                        }
                        else if (base == 16 && c >= 'A' && c <= 'F')
                        {
                            thisCharsValue = (c - 'A') + 10;
                        } 
                        else if (base == 16 && c >= 'a' && c <= 'f')
                        {
                        	thisCharsValue = (c - 'a') + 10;
                        }
                        else
                        {
                        	break; // No idea how to handle this.
                        }
                        
                        /* Increase our total. */
                        charValue = base * charValue + thisCharsValue;
                    }

                    c = (char)charValue;
                    i--;
                }
                else
                {
                	switch(c)
                	{
	                	case 'a':  c = '\007'; break;
	                	case 'b':  c = '\b';   break;
	                	case 'f':  c = '\f';   break;
	                	case 'n':  c = '\n';   break;
	                	case 'r':  c = '\r';   break;
	                	case 't':  c = '\t';   break;
	                	case 'v':  c = '\013'; break;
	                	case '"':  c = '"';    break;
	                	case '\'': c = '\'';   break;
	                	case '\\': c = '\\';   break;
	               	 /* default:   c = c;      break; */
                	}
                }
            }
            result += c;
        }

        return result;
    }

    private String scanToken()
    {
    	/* Grab the next character. If there isn't one, we're done. */
        int ch = scanChar();
        if (ch == -1)
        {
            return "";
        }
        
        /* If this is a digit and we're supposed to scan numbers as strings, scan a number. */
        if (scanNumbersFlag && Character.isDigit(ch))
        {
            ungetChar(ch);
            return scanNumber();
        }
        
        /* If this is part of a word, scan a whole word. */
        if (isWordCharacter(ch))
        {
            ungetChar(ch);
            return scanWord();
        }
        
        /* If we're supposed to scan strings as though they're individual tokens, read
         * a full string and return it.
         */
        if (scanStringsFlag && (ch == '\'' || ch == '\"'))
        {
            ungetChar(ch);
            return scanString();
        }
        
        String s = "" + (char)ch;
        
        while (true)
        {
            if(!isOperatorPrefix(s))
                break;
            
            int j = getChar();
            if(j == -1)
                break;
            
            s += j;
        }
        
        
        for(; s.length() > 1 && !isOperator(s); s = s.substring(0, s.length() - 1))
            ungetChar(s.charAt(s.length() - 1));

        return s;
    }

    private int scanChar()
    {
        int i;
label0:
        {
            boolean flag = false;
            boolean flag1 = false;
            int k;
            while (true)
            {
                do
                {
                    i = getChar();
                }
                while (ignoreWhitespaceFlag && Character.isWhitespace(i) && !isOperator(Character.toString((char)i)));
                if(flag)
                {
                    int j = getChar();
                    if(i == 42 && j == 47)
                        flag = false;
                    else
                        ungetChar(j);
                    continue;
                }
                if(flag1)
                {
                    if(i == 10 || i == 13)
                        flag1 = false;
                    continue;
                }
                if(!ignoreCommentsFlag || i != 47)
                    break label0;
                k = getChar();
                if(k == 42)
                {
                    flag = true;
                    continue;
                }
                if(k != 47)
                    break;
                flag1 = true;
            }
            ungetChar(k);
        }
        return i;
    }

    private String scanWord()
    {
        String s = "";
        int i;
        for(i = getChar(); isWordCharacter(i); i = getChar())
            s = (new StringBuilder()).append(s).append((char)i).toString();

        ungetChar(i);
        return s;
    }

    private String scanNumber()
    {
        String result = "";
        int state = INITIAL_STATE;
        int exponentSign = 'E';
        
        while (true)
        {
            if(state == FINAL_STATE)
                break;
            
            int nextChar = getChar();
            switch(state)
            {
            case INITIAL_STATE:
                if(nextChar == '0')
                {
                    int possibleHexMarker = getChar();
                    if(possibleHexMarker == 'x' || possibleHexMarker == 'X')
                    {
                        result = "0x";
                        continue;
                    }
                    ungetChar(possibleHexMarker);
                }
                state = BEFORE_DECIMAL_POINT;
                break;

            case BEFORE_DECIMAL_POINT:
                if(nextChar == '.')
                    state = AFTER_DECIMAL_POINT;
                else if(nextChar == 'E' || nextChar == 'e')
                    state = STARTING_EXPONENT;
                else if(!Character.isDigit(nextChar))
                    state = FINAL_STATE;
                break;

            case AFTER_DECIMAL_POINT:
                if(nextChar == 'E' || nextChar == 'e')
                {
                    exponentSign = nextChar;
                    state = STARTING_EXPONENT;
                }
                else if(!Character.isDigit(nextChar))
                    state = FINAL_STATE;
                break;

            case STARTING_EXPONENT:
                if(nextChar == '+' || nextChar == '-')
                    state = FOUND_EXPONENT_SIGN;
                else if(Character.isDigit(nextChar))
                    state = SCANNING_EXPONENT;
                else
                    state = FINAL_STATE;
                break;

            case FOUND_EXPONENT_SIGN:
                if(Character.isDigit(nextChar))
                {
                    state = SCANNING_EXPONENT;
                }
                else
                {
                    ungetChar(nextChar);
                    nextChar = exponentSign;
                    state = FINAL_STATE;
                }
                break;

            case SCANNING_EXPONENT:
                if(!Character.isDigit(nextChar))
                    state = FINAL_STATE;
                break;
            }
            if(state == FINAL_STATE)
            {
                ungetChar(nextChar);
                break;
            }
            result += (char)nextChar;
        }
        return result;
    }

    /* Scans a quoted string and returns it as a unit. */
    private String scanString()
    {
    	/* The delimiter can be either ' or ", but we won't know which until we scan the first
    	 * character.
    	 */
        char delimiter = (char)getChar();
        String result = "" + delimiter;
        
        /* Keep scanning characters until we find a close quote that isn't escaped.
         * Although normally we don't peek inside of strings, we have to do that here
         * so that if we have a string that ends with \\", we properly treat that as
         * an escaped slash followed by a close quote.
         */
        while (true)
        {
            int ch = getChar();
            
            /* If this is an escape sequence start, put it, and whatever it's escaping, into the
             * resulting string.
             */
            if (ch == '\\')
            {
            	result += (char) ch;
            	
            	/* Add the next character, if one exists. */
            	int next = getChar();
            	if (next == -1) break;
            	
            	result += (char) next;
            }
            /* Otherwise, if this isn't an escape sequence and it isn't the delimiter, drop it into the result. */
            else if (ch != -1 && ch != delimiter)
            {
            	result += (char) ch;
            }
            /* Otherwise, this either is the delimiter or we're out of characters. */
            else break;
        }
        
        return result + delimiter;
    }

    private boolean isOperator(String s)
    {
        for(int i = 0; i < operators.size(); i++)
            if(((String)operators.get(i)).equals(s))
                return true;

        return false;
    }

    private boolean isOperatorPrefix(String s)
    {
        for(int i = 0; i < operators.size(); i++)
            if(((String)operators.get(i)).startsWith(s))
                return true;

        return false;
    }

    public static final int EOF = -1;
    public static final int SEPARATOR = 0;
    public static final int WORD = 1;
    public static final int NUMBER = 2;
    public static final int STRING = 3;
    public static final int OPERATOR = 4;
    private static final int INITIAL_STATE = 0;
    private static final int BEFORE_DECIMAL_POINT = 1;
    private static final int AFTER_DECIMAL_POINT = 2;
    private static final int STARTING_EXPONENT = 3;
    private static final int FOUND_EXPONENT_SIGN = 4;
    private static final int SCANNING_EXPONENT = 5;
    private static final int FINAL_STATE = 6;
    private Reader input;
    private String wordChars;
    private String savedChars;
    private Stack<String> savedTokens;
    private ArrayList<String> operators;
    private int cpos;
    private boolean ignoreWhitespaceFlag;
    private boolean ignoreCommentsFlag;
    private boolean scanNumbersFlag;
    private boolean scanStringsFlag;
}
