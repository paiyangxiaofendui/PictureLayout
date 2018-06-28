using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
//using System.Text;
using System.Net;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;

//using System.Windows;
using System.Windows.Forms;
//using System.Windows.Forms.MessageBox;
using System.Runtime.InteropServices;

namespace SMSSender
{
    public class CSMSSender
    {
        private const String host = "http://ali-sms.showapi.com";
        private const String path = "/sendSms";
        private const String method = "GET";
        private const String appcode = /*"3350663072574e78bea68a80b793eac0"*/"4a7fa19314ca4fe0a24d9d4e9829ced9";

        [DllImport("winInet.dll ")]
        private static extern bool InternetGetConnectedState(ref int Flag, int dwReserved); 

        public int SendStringToPhoneFunc(String strPhoneNum, String strToSend)
        {
            int nFlag = 0;
            if(!InternetGetConnectedState(ref nFlag, 0))
                return 10006;

            //String querys = "content=%7B%22code%22%3A%221%22%2C%22minute%22%3A%225%22%2C%22comName%22%3A%22321%22%7D&mobile=13632418997&tNum=T150606060601";
            //String querys = "content=%7B%22code%22%3A%22" + strToSend + "%22%2C%22minute%22%3A%225%22%2C%22comName%22%3A%22321%22%7D&mobile=" + strPhoneNum + "&tNum=T150606060601";
            String querys = "content=%7B%22code%22%3A%22" + strToSend + "%22%7D&mobile=" + strPhoneNum + "&tNum=T170317001192";

            //MessageBox.Show(querys, "错误", MessageBoxButtons.OK, MessageBoxIcon.Error); 

            String bodys = "";
            String url = host + path;
            HttpWebRequest httpRequest = null;
            HttpWebResponse httpResponse = null;

            if (0 < querys.Length)
            {
                url = url + "?" + querys;
            }

            if (host.Contains("https://"))
            {
                ServicePointManager.ServerCertificateValidationCallback = new RemoteCertificateValidationCallback(CheckValidationResult);
                httpRequest = (HttpWebRequest)WebRequest.CreateDefault(new Uri(url));
            }
            else
            {
                httpRequest = (HttpWebRequest)WebRequest.Create(url);
            }
            httpRequest.Method = method;
            httpRequest.Headers.Add("Authorization", "APPCODE " + appcode);
            if (0 < bodys.Length)
            {
                byte[] data = Encoding.UTF8.GetBytes(bodys);
                using (Stream stream = httpRequest.GetRequestStream())
                {
                    stream.Write(data, 0, data.Length);
                }
            }
            try
            {
                //MessageBox.Show("before get", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                httpResponse = (HttpWebResponse)httpRequest.GetResponse();
                //MessageBox.Show("After get", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            catch (WebException ex)
            {
                httpResponse = (HttpWebResponse)ex.Response;
                //MessageBox.Show(httpResponse.StatusCode.ToString(), "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            catch (Exception e)
            {
                //MessageBox.Show("Other Exception", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            //Console.WriteLine(httpResponse.StatusCode);
            //Console.WriteLine(httpResponse.Method);
            //Console.WriteLine(httpResponse.Headers);
            //Stream st = httpResponse.GetResponseStream();
            //StreamReader reader = new StreamReader(st, Encoding.GetEncoding("utf-8"));
            //Console.WriteLine(reader.ReadToEnd());
            //Console.WriteLine("\n");

            //MessageBox.Show(httpResponse.StatusCode.ToString(), "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //MessageBox.Show(httpResponse.Method, "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);

            Stream st = httpResponse.GetResponseStream();
            StreamReader reader = new StreamReader(st, Encoding.GetEncoding("utf-8"));
            String strRes = reader.ReadToEnd();
            //MessageBox.Show(strRes, "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);

            return 0;
        }

        public static bool CheckValidationResult(object sender, X509Certificate certificate, X509Chain chain, SslPolicyErrors errors)
        {
            return true;
        }
    }
}
