using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;

namespace CSharpTest
{
    public partial class Form1 : Form
    {
        [DllImport("Algorithm_CSharpd.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "LayoutOptimize", CharSet = CharSet.Ansi)]
        extern static int LayoutOptimize();

        [DllImport("Algorithm_CSharpd.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Test", CharSet = CharSet.Ansi)]
        public static extern int Test(char[] hello);


        [DllImport("Algorithm_CSharpd", CallingConvention = CallingConvention.Cdecl, EntryPoint = "LayoutOptimizeByStream", CharSet = CharSet.Ansi)]
        public static extern IntPtr LayoutOptimizeByStream([MarshalAs(UnmanagedType.LPStr)]string inString);  
  



        [DllImport("Encryptiond.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "encrypt_base64", CharSet = CharSet.Ansi)]
        public static extern int encrypt_base64(char[] src_path, char[] dst_path);


        [DllImport("Encryptiond.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "decrypt_base64", CharSet = CharSet.Ansi)]
        public static extern int decrypt_base64(char[] src_path, char[] dst_path);



        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //int rt;

            Read();


            //string str = "hello world!";

            // rt = Test(str);
            
            // string src_file = "C:\\Users\\Administrator\\Desktop\\C#—孔槽翻转测试.xml";
            // string dst_file = "C:\\Users\\Administrator\\Desktop\\C#—孔槽翻转测试.hge";
            // string dst_xml_file = "C:\\Users\\Administrator\\Desktop\\C#—孔槽翻转测试2.xml";
            // rt = encrypt_base64(src_file.ToCharArray(), dst_file.ToCharArray());
            // rt = decrypt_base64(dst_file.ToCharArray(), dst_xml_file.ToCharArray());

        }

       
        public void Read()
        {
            try
            {
                //string src_file = "E:\\新雕刻机\\trunk\\bin\\unseodata.xml";

                FileStream file = new FileStream("E:\\新雕刻机\\trunk\\bin\\unseodata.xml", FileMode.Open);
                int file_len = (int)(file.Length);


                byte[] byData = new byte[file_len];
                char[] charData = new char[file_len];

                file.Seek(0, SeekOrigin.Begin);
                file.Read(byData, 0, file_len); //byData传进来的字节数组,用以接受FileStream对象中的数据,第2个参数是字节数组中开始写入数据的位置,它通常是0,表示从数组的开端文件中向数组写数据,最后一个参数规定从文件读多少字符.
               
                Decoder d = Encoding.Default.GetDecoder();
               
                d.GetChars(byData, 0, byData.Length, charData, 0);

               // Console.WriteLine(charData);

                file.Close();

                string strContent = new string(charData);

                //Ansi  
                IntPtr ip = LayoutOptimizeByStream(strContent);
                string strIP = Marshal.PtrToStringAnsi(ip);
                Console.WriteLine(strIP);


                MessageBox.Show(strIP);


                //return charData;
                //file_content = new string(charData);

            }
            catch (IOException e)
            {
                Console.WriteLine(e.ToString());
            }
        }



    }
}
