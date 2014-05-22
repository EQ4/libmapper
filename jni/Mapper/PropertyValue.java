
package Mapper;
import java.util.Arrays;

/* Contains a value representable by supported types, which correspond
 * with OSC types. */
public class PropertyValue
{
    public PropertyValue()                       { length=0; type = 0; }
    public PropertyValue(int i)                  { setValue('i', i);   }
    public PropertyValue(float f)                { setValue('f', f);   }
    public PropertyValue(double d)               { setValue('d', d);   }
    public PropertyValue(String s)               { setValue('s', s);   }
    public PropertyValue(int[] i)                { setValue('i', i);   }
    public PropertyValue(float[] f)              { setValue('f', f);   }
    public PropertyValue(double[] d)             { setValue('d', d);   }
    public PropertyValue(String[] s)             { setValue('s', s);   }
    public PropertyValue(char _type, int i)      { setValue(_type, i); }
    public PropertyValue(char _type, float f)    { setValue(_type, f); }
    public PropertyValue(char _type, double d)   { setValue(_type, d); }
    public PropertyValue(char _type, String s)   { setValue(_type, s); }
    public PropertyValue(char _type, int[] i)    { setValue(_type, i); }
    public PropertyValue(char _type, float[] f)  { setValue(_type, f); }
    public PropertyValue(char _type, double[] d) { setValue(_type, d); }
    public PropertyValue(char _type, String[] s) { setValue(_type, s); }

    // TODO handle entire set of OSC types
    public class PropertyException extends RuntimeException {
        public PropertyException() {
            super("Property does not contain requested type.");
        }
        public PropertyException(String msg) {
            super(msg);
        }
    }

    public Object value() {
        switch (type) {
            case 'i': {
                if (length == 1)
                    return new Integer(_i[0]);
                else if (length > 1) {
                    int[] v = _i.clone();
                    return v;
                }
            }
            case 'f': {
                if (length == 1)
                    return new Float(_f[0]);
                else if (length > 1) {
                    float[] v = _f.clone();
                    return v;
                }
            }
            case 'd': {
                if (length == 1)
                    return new Double(_d[0]);
                else if (length > 1) {
                    double[] v = _d.clone();
                    return v;
                }
            }
            case 's': {
                if (length == 1)
                    return _s[0];
                else if (length > 1) {
                    String[] v = _s.clone();
                    return v;
                }
            }
            case 'S': {
                if (length == 1)
                    return _s[0];
                else if (length > 1) {
                    String[] v = _s.clone();
                    return v;
                }
            }
        }
        return null;
    }

    public int intValue() {
        if (type == 'i')
            return _i[0];
        throw new PropertyException();
    }

    public float floatValue() {
        if (type == 'f')
            return _f[0];
        else if (type == 'i')
            return _i[0];
        throw new PropertyException();
    }

    public double doubleValue() {
        if (type == 'd')
            return _d[0];
        else if (type == 'f')
            return _f[0];
        else if (type == 'i')
            return _i[0];
        throw new PropertyException();
    }

    public String stringValue() {
        if (type == 's' || type == 'S')
            return _s[0];
        throw new PropertyException();
    }

    public void setValue(char _type, int i) {
        switch (_type) {
            case 'i':
                _i = new int[1];
                _i[0] = i;
                break;
            case 'f':
                _f = new float[1];
                _f[0] = i;
                break;
            case 'd':
                _d = new double[1];
                _d[0] = i;
                break;
            case 's':
                _s = new String[1];
                _s[0] = String.valueOf(i);
                break;
            case 'S':
                _s = new String[1];
                _s[0] = String.valueOf(i);
                break;
            default:
                throw new PropertyException("Cannot cast int to requested type.");
        }
        type = _type;
        length = 1;
    }

    public void setValue(char _type, float f) {
        switch (_type) {
            case 'i':
                _i = new int[1];
                _i[1] = (int)f;
                break;
            case 'f':
                _f = new float[1];
                _f[0] = f;
                break;
            case 'd':
                _d = new double[1];
                _d[0] = f;
                break;
            case 's':
                _s = new String[1];
                _s[0] = String.valueOf(f);
                break;
            case 'S':
                _s = new String[1];
                _s[0] = String.valueOf(f);
                break;
            default:
                throw new PropertyException("Cannot cast float to requested type.");
        }
        type = _type;
        length = 1;
    }

    public void setValue(char _type, double d) {
        switch (_type) {
            case 'i':
                _i = new int[1];
                _i[0] = (int)d;
                break;
            case 'f':
                _f = new float[1];
                _f[0] = (float)d;
                break;
            case 'd':
                _d = new double[1];
                _d[0] = d;
                break;
            case 's':
                _s = new String[1];
                _s[0] = String.valueOf(d);
                break;
            case 'S':
                _s = new String[1];
                _s[0] = String.valueOf(d);
                break;
            default:
                throw new PropertyException("Cannot cast double to requested type.");
        }
        type = _type;
        length = 1;
    }

    public void setValue(char _type, String s) {
        switch (_type) {
            case 'i':
                _i = new int[1];
                _i[0] = Integer.parseInt(s);
                break;
            case 'f':
                _f = new float[1];
                _f[0] = Float.parseFloat(s);
                break;
            case 'd':
                _d = new double[1];
                _d[0] = Double.parseDouble(s);
                break;
            case 's':
                _s = new String[1];
                _s[0] = s;
                break;
            case 'S':
                _s = new String[1];
                _s[0] = s;
                break;
            default:
                throw new PropertyException("Cannot cast String to requested type.");
        }
        type = _type;
        length = 1;
    }

    public void setValue(char _type, int[] i) {
        switch (_type) {
            case 'i':
                _i = i.clone();
                break;
            case 'f':
                _f = new float[i.length];
                for (int n = 0; n < i.length; n++)
                    _f[n] = i[n];
                break;
            case 'd':
                _d = new double[i.length];
                for (int n = 0; n < i.length; n++)
                    _d[n] = i[n];
                break;
            case 's':
                _s = new String[i.length];
                for (int n = 0; n < i.length; n++)
                    _s[n] = String.valueOf(i[n]);
                break;
            case 'S':
                _s = new String[i.length];
                for (int n = 0; n < i.length; n++)
                    _s[n] = String.valueOf(i[n]);
                break;
            default:
                throw new PropertyException("Cannot cast int to requested type.");
        }
        type = _type;
        length = i.length;
    }
    
    public void setValue(char _type, float[] f) {
        switch (_type) {
            case 'i':
                _i = new int[f.length];
                for (int n = 0; n < f.length; n++)
                    _i[n] = (int)f[n];
                break;
            case 'f':
                _f = f.clone();
                break;
            case 'd':
                _d = new double[f.length];
                for (int n = 0; n < f.length; n++)
                    _d[n] = f[n];
                break;
            case 's':
                _s = new String[f.length];
                for (int n = 0; n < f.length; n++)
                    _s[n] = String.valueOf(f[n]);
                break;
            case 'S':
                _s = new String[f.length];
                for (int n = 0; n < f.length; n++)
                    _s[n] = String.valueOf(f[n]);
                break;
            default:
                throw new PropertyException("Cannot cast float to requested type.");
        }
        type = _type;
        length = f.length;
    }
    
    public void setValue(char _type, double[] d) {
        switch (_type) {
            case 'i':
                _i = new int[d.length];
                for (int n = 0; n < d.length; n++)
                    _i[n] = (int)d[n];
                break;
            case 'f':
                _f = new float[d.length];
                for (int n = 0; n < d.length; n++)
                    _f[n] = (float)d[n];
                break;
            case 'd':
                _d = d.clone();
                break;
            case 's':
                _s = new String[d.length];
                for (int n = 0; n < d.length; n++)
                    _s[n] = String.valueOf(d[n]);
                break;
            case 'S':
                _s = new String[d.length];
                for (int n = 0; n < d.length; n++)
                    _s[n] = String.valueOf(d[n]);
                break;
            default:
                throw new PropertyException("Cannot cast double to requested type.");
        }
        type = _type;
        length = d.length;
    }
    
    public void setValue(char _type, String[] s) {
        switch (_type) {
            case 'i':
                _i = new int[s.length];
                for (int n = 0; n < s.length; n++)
                    _i[n] = Integer.parseInt(s[n]);
                break;
            case 'f':
                _f = new float[s.length];
                for (int n = 0; n < s.length; n++)
                    _f[n] = Float.parseFloat(s[n]);
                break;
            case 'd':
                _d = new double[s.length];
                for (int n = 0; n < s.length; n++)
                    _d[n] = Double.parseDouble(s[n]);
                break;
            case 's':
                _s = s.clone();
            case 'S':
                _s = s.clone();
                break;
            default:
                throw new PropertyException("Cannot cast String to requested type.");
        }
        type = _type;
        length = s.length;
    }

    public String toString() {
        String s;
        switch (type) {
            case 'i': s = Arrays.toString(_i); break;
            case 'f': s = Arrays.toString(_f); break;
            case 'd': s = Arrays.toString(_d); break;
            case 's': s = Arrays.toString(_s); break;
            case 'S': s = Arrays.toString(_s); break;
            default: s = null;
        }
        return "<type="+type+", length="+length+", value="+s+">";
    }

    public char type;
    public int length;

    private int[] _i;
    private float[] _f;
    private double[] _d;
    private String[] _s;
}
