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
        savedTokens = new Stack();
        operators = new ArrayList();
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
        int i = 0;
_L3:
        if(i >= s.length()) goto _L2; else goto _L1
_L1:
        char c;
        c = s.charAt(i);
        if(wordChars.indexOf(c) != -1)
            continue; /* Loop/switch isn't completed */
        new StringBuilder();
        this;
        JVM INSTR dup_x1 ;
        wordChars;
        append();
        c;
        append();
        toString();
        wordChars;
        i++;
          goto _L3
_L2:
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

    public int getChar()
    {
        char c;
        cpos++;
        if(savedChars.isEmpty())
            break MISSING_BLOCK_LABEL_43;
        c = savedChars.charAt(0);
        savedChars = savedChars.substring(1);
        return c;
        return input.read();
        IOException ioexception;
        ioexception;
        throw new ErrorException(ioexception);
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

    public String getStringValue(String s)
    {
        String s1 = "";
        boolean flag = false;
        int i = s.length();
        if(i > 1 && (s.charAt(0) == '"' || s.charAt(0) == '\''))
        {
            flag = true;
            i--;
        }
        for(int j = ((flag) ? 1 : 0); j < i; j++)
        {
            char c = s.charAt(j);
            if(c == '\\')
            {
                c = s.charAt(++j);
                if(Character.isDigit(c) || c == 'x')
                {
                    byte byte0 = 8;
                    byte byte1 = 3;
                    if(c == 'x')
                    {
                        byte0 = 16;
                        byte1 = 2;
                        j++;
                    }
                    int k = 0;
                    boolean flag1 = false;
                    for(int i1 = Math.min(i, j + byte1); j < i1; j++)
                    {
                        c = s.charAt(j);
                        int l;
                        if(Character.isDigit(c))
                            l = c - 48;
                        else
                        if(byte0 == 16 && c >= 'A' && c <= 'F')
                        {
                            l = (c - 65) + 10;
                        } else
                        {
                            if(byte0 != 16 || c < 'a' || c > 'f')
                                break;
                            l = (c - 97) + 10;
                        }
                        k = byte0 * k + l;
                    }

                    c = (char)k;
                    j--;
                } else
                {
                    switch(c)
                    {
                    case 97: // 'a'
                        c = '\007';
                        break;

                    case 98: // 'b'
                        c = '\b';
                        break;

                    case 102: // 'f'
                        c = '\f';
                        break;

                    case 110: // 'n'
                        c = '\n';
                        break;

                    case 114: // 'r'
                        c = '\r';
                        break;

                    case 116: // 't'
                        c = '\t';
                        break;

                    case 118: // 'v'
                        c = '\013';
                        break;

                    case 34: // '"'
                        c = '"';
                        break;

                    case 39: // '\''
                        c = '\'';
                        break;

                    case 92: // '\\'
                        c = '\\';
                        break;
                    }
                }
            }
            s1 = (new StringBuilder()).append(s1).append(c).toString();
        }

        return s1;
    }

    private String scanToken()
    {
        int i = scanChar();
        if(i == -1)
            return "";
        if(scanNumbersFlag && Character.isDigit(i))
        {
            ungetChar(i);
            return scanNumber();
        }
        if(isWordCharacter(i))
        {
            ungetChar(i);
            return scanWord();
        }
        if(scanStringsFlag && (i == 34 || i == 39))
        {
            ungetChar(i);
            return scanString();
        }
        String s = (new StringBuilder()).append("").append((char)i).toString();
        do
        {
            if(!isOperatorPrefix(s))
                break;
            int j = getChar();
            if(j == -1)
                break;
            s = (new StringBuilder()).append(s).append((char)j).toString();
        } while(true);
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
            do
            {
                do
                    i = getChar();
                while(ignoreWhitespaceFlag && Character.isWhitespace(i) && !isOperator(Character.toString((char)i)));
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
            } while(true);
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
        String s = "";
        int i = 0;
        int j = 69;
        do
        {
            if(i == 6)
                break;
            int k = getChar();
            switch(i)
            {
            case 0: // '\0'
                if(k == 48)
                {
                    int l = getChar();
                    if(l == 120 || l == 88)
                    {
                        s = "0x";
                        continue;
                    }
                    ungetChar(l);
                }
                i = 1;
                break;

            case 1: // '\001'
                if(k == 46)
                    i = 2;
                else
                if(k == 69 || k == 101)
                    i = 3;
                else
                if(!Character.isDigit(k))
                    i = 6;
                break;

            case 2: // '\002'
                if(k == 69 || k == 101)
                {
                    j = k;
                    i = 3;
                } else
                if(!Character.isDigit(k))
                    i = 6;
                break;

            case 3: // '\003'
                if(k == 43 || k == 45)
                    i = 4;
                else
                if(Character.isDigit(k))
                    i = 5;
                else
                    i = 6;
                break;

            case 4: // '\004'
                if(Character.isDigit(k))
                {
                    i = 5;
                } else
                {
                    ungetChar(k);
                    k = j;
                    i = 6;
                }
                break;

            case 5: // '\005'
                if(!Character.isDigit(k))
                    i = 6;
                break;
            }
            if(i == 6)
            {
                ungetChar(k);
                break;
            }
            s = (new StringBuilder()).append(s).append((char)k).toString();
        } while(true);
        return s;
    }

    private String scanString()
    {
        char c = (char)getChar();
        String s = (new StringBuilder()).append("").append(c).toString();
        int i = 0;
        do
        {
            int j = getChar();
            if(j != -1 && (j != c || i == 92))
            {
                s = (new StringBuilder()).append(s).append((char)j).toString();
                i = j;
            } else
            {
                return (new StringBuilder()).append(s).append(c).toString();
            }
        } while(true);
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
    private Stack savedTokens;
    private ArrayList operators;
    private int cpos;
    private boolean ignoreWhitespaceFlag;
    private boolean ignoreCommentsFlag;
    private boolean scanNumbersFlag;
    private boolean scanStringsFlag;
}
