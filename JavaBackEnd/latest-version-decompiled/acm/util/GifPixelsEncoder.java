// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   MediaTools.java

package acm.util;

import java.io.IOException;
import java.io.OutputStream;

class GifPixelsEncoder
{

    GifPixelsEncoder(int i, int j, byte abyte0[], boolean flag, int k)
    {
        maxbits = 12;
        maxmaxcode = 4096;
        htab = new int[5003];
        codetab = new int[5003];
        hsize = 5003;
        free_ent = 0;
        clear_flg = false;
        cur_accum = 0;
        cur_bits = 0;
        accum = new byte[256];
        imgW = i;
        imgH = j;
        pixAry = abyte0;
        wantInterlaced = flag;
        initCodeSize = Math.max(2, k);
    }

    void encode(OutputStream outputstream)
        throws IOException
    {
        outputstream.write(initCodeSize);
        countDown = imgW * imgH;
        xCur = yCur = curPass = 0;
        compress(initCodeSize + 1, outputstream);
        outputstream.write(0);
    }

    private void bumpPosition()
    {
        xCur++;
        if(xCur == imgW)
        {
            xCur = 0;
            if(!wantInterlaced)
                yCur++;
            else
                switch(curPass)
                {
                default:
                    break;

                case 0: // '\0'
                    yCur += 8;
                    if(yCur >= imgH)
                    {
                        curPass++;
                        yCur = 4;
                    }
                    break;

                case 1: // '\001'
                    yCur += 8;
                    if(yCur >= imgH)
                    {
                        curPass++;
                        yCur = 2;
                    }
                    break;

                case 2: // '\002'
                    yCur += 4;
                    if(yCur >= imgH)
                    {
                        curPass++;
                        yCur = 1;
                    }
                    break;

                case 3: // '\003'
                    yCur += 2;
                    break;
                }
        }
    }

    private int nextPixel()
    {
        if(countDown == 0)
        {
            return -1;
        } else
        {
            countDown--;
            byte byte0 = pixAry[yCur * imgW + xCur];
            bumpPosition();
            return byte0 & 0xff;
        }
    }

    final int MAXCODE(int i)
    {
        return (1 << i) - 1;
    }

    void compress(int i, OutputStream outputstream)
        throws IOException
    {
        g_init_bits = i;
        clear_flg = false;
        n_bits = g_init_bits;
        maxcode = MAXCODE(n_bits);
        ClearCode = 1 << i - 1;
        EOFCode = ClearCode + 1;
        free_ent = ClearCode + 2;
        char_init();
        int j1 = nextPixel();
        int i2 = 0;
        for(int j = hsize; j < 0x10000; j *= 2)
            i2++;

        i2 = 8 - i2;
        int l1 = hsize;
        cl_hash(l1);
        output(ClearCode, outputstream);
label0:
        do
        {
            int i1;
            if((i1 = nextPixel()) == -1)
                break;
            int k = (i1 << maxbits) + j1;
            int l = i1 << i2 ^ j1;
            if(htab[l] == k)
            {
                j1 = codetab[l];
                continue;
            }
            if(htab[l] >= 0)
            {
                int k1 = l1 - l;
                if(l == 0)
                    k1 = 1;
                do
                {
                    if((l -= k1) < 0)
                        l += l1;
                    if(htab[l] != k)
                        continue;
                    j1 = codetab[l];
                    continue label0;
                } while(htab[l] >= 0);
            }
            output(j1, outputstream);
            j1 = i1;
            if(free_ent < maxmaxcode)
            {
                codetab[l] = free_ent++;
                htab[l] = k;
            } else
            {
                cl_block(outputstream);
            }
        } while(true);
        output(j1, outputstream);
        output(EOFCode, outputstream);
    }

    void output(int i, OutputStream outputstream)
        throws IOException
    {
        cur_accum &= masks[cur_bits];
        if(cur_bits > 0)
            cur_accum |= i << cur_bits;
        else
            cur_accum = i;
        for(cur_bits += n_bits; cur_bits >= 8; cur_bits -= 8)
        {
            char_out((byte)(cur_accum & 0xff), outputstream);
            cur_accum >>= 8;
        }

        if(free_ent > maxcode || clear_flg)
            if(clear_flg)
            {
                maxcode = MAXCODE(n_bits = g_init_bits);
                clear_flg = false;
            } else
            {
                n_bits++;
                if(n_bits == maxbits)
                    maxcode = maxmaxcode;
                else
                    maxcode = MAXCODE(n_bits);
            }
        if(i == EOFCode)
        {
            for(; cur_bits > 0; cur_bits -= 8)
            {
                char_out((byte)(cur_accum & 0xff), outputstream);
                cur_accum >>= 8;
            }

            flush_char(outputstream);
        }
    }

    void cl_block(OutputStream outputstream)
        throws IOException
    {
        cl_hash(hsize);
        free_ent = ClearCode + 2;
        clear_flg = true;
        output(ClearCode, outputstream);
    }

    void cl_hash(int i)
    {
        for(int j = 0; j < i; j++)
            htab[j] = -1;

    }

    void char_init()
    {
        a_count = 0;
    }

    void char_out(byte byte0, OutputStream outputstream)
        throws IOException
    {
        accum[a_count++] = byte0;
        if(a_count >= 254)
            flush_char(outputstream);
    }

    void flush_char(OutputStream outputstream)
        throws IOException
    {
        if(a_count > 0)
        {
            outputstream.write(a_count);
            outputstream.write(accum, 0, a_count);
            a_count = 0;
        }
    }

    private static final int EOF = -1;
    private int imgW;
    private int imgH;
    private byte pixAry[];
    private boolean wantInterlaced;
    private int initCodeSize;
    private int countDown;
    private int xCur;
    private int yCur;
    private int curPass;
    static final int BITS = 12;
    static final int HSIZE = 5003;
    int n_bits;
    int maxbits;
    int maxcode;
    int maxmaxcode;
    int htab[];
    int codetab[];
    int hsize;
    int free_ent;
    boolean clear_flg;
    int g_init_bits;
    int ClearCode;
    int EOFCode;
    int cur_accum;
    int cur_bits;
    int masks[] = {
        0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 
        1023, 2047, 4095, 8191, 16383, 32767, 65535
    };
    int a_count;
    byte accum[];
}
