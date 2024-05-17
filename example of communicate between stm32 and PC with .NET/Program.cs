using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;

class Program
{
    static UdpClient receiver;
    static IPEndPoint RemoteEndPoint = new IPEndPoint(IPAddress.Parse("192.168.1.1"), 5001); // STM32的IP和端口
    static ManualResetEventSlim stopReceiver = new ManualResetEventSlim(false);

    static void Main(string[] args)
    {
        receiver = new UdpClient(5002);

        // 创建并启动接收数据的线程
        Thread receiveThread = new Thread(ReceiveData);
        receiveThread.Start();

        // 创建并启动定时发送数据的线程
        Thread sendThread = new Thread(SendDataPeriodically);
        sendThread.Start();

        Console.WriteLine("按任意键退出...");
        Console.ReadKey();

        // 请求线程停止
        stopReceiver.Set();

        // 等待线程结束
        receiveThread.Join();
        sendThread.Join();

        receiver.Close();
    }

    static void ReceiveData()
    {
        while (!stopReceiver.IsSet)
        {
            try
            {
                byte[] data = receiver.Receive(ref RemoteEndPoint);
                //Console.WriteLine($"接收到数据的来源: {RemoteEndPoint.Address}:{RemoteEndPoint.Port}");

                Console.Write($"STM32 -> PC:({RemoteEndPoint.Address}:{RemoteEndPoint.Port})  ");
                for (int i = 0; i < data.Length; i++)
                {
                    Console.Write("{0:x2} ", data[i]);
                }
                Console.WriteLine(); // 换行
            }
            catch (ObjectDisposedException) // 可能在关闭receiver时抛出
            {
                return;
            }
            catch (SocketException ex)
            {
                Console.WriteLine($"Socket异常: {ex.Message}");
            }
        }
    }

    static void SendDataPeriodically()
    {
        //string messageToSend = "Hello from PC!";
        //byte[] sendData = Encoding.UTF8.GetBytes(messageToSend);
        string sourceIp = "192.168.1.2"; // 示例IP，根据实际情况替换
        int sourcePort = 5002; // 假定的发送端口，如果是从特定端口发送则应获取该端口
        byte[] sendData = {0xFF,0xFF, 0xFF, 0xFF };
        string messageToSend = ByteArrayToHexadecimalString(sendData);
        while (!stopReceiver.IsSet)
        {
            try
            {
                receiver.Send(sendData, sendData.Length, RemoteEndPoint);
                //Console.WriteLine($"PC -> STM32:({sourceIp}:{sourcePort})  {messageToSend}");
                Console.WriteLine($"\u001b[33mPC -> STM32:({sourceIp}:{sourcePort})  {messageToSend}\u001b[0m");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"发送失败: {ex.Message}");
            }

            Thread.Sleep(4000); // 每隔2秒发送一次
        }
    }

    public static string ByteArrayToHexadecimalString(byte[] byteArray)
    {
        StringBuilder hexString = new StringBuilder(byteArray.Length * 2);
        foreach (byte b in byteArray)
        {
            hexString.Append(b.ToString("X2"));
            hexString.Append(" ");
        }
        return hexString.ToString();
    }
}