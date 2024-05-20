using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Net.NetworkInformation;

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
        //string sourceIp = "192.168.1.2"; // 示例IP，根据实际情况替换
        string localIpAddress = GetEthernetLocalIPAddress();
        int sourcePort = 5002; // 假定的发送端口，如果是从特定端口发送则应获取该端口
        byte[] sendData = {0xFF,0xFF, 0xFF, 0xFF };
        string messageToSend = ByteArrayToHexadecimalString(sendData);
        while (!stopReceiver.IsSet)
        {
            try
            {
                receiver.Send(sendData, sendData.Length, RemoteEndPoint);
                //Console.WriteLine($"PC -> STM32:({sourceIp}:{sourcePort})  {messageToSend}");
                Console.WriteLine($"\u001b[33mPC -> STM32:({localIpAddress}:{sourcePort})  {messageToSend}\u001b[0m");
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

    public static string GetEthernetLocalIPAddress()
    {
        var interfaces = NetworkInterface.GetAllNetworkInterfaces();
        foreach (var network in interfaces)
        {
            // 确保网络接口处于工作状态且是 Ethernet 类型
            if (network.OperationalStatus != OperationalStatus.Up || network.NetworkInterfaceType != NetworkInterfaceType.Ethernet)
                continue;

            var properties = network.GetIPProperties();
            if (properties.GatewayAddresses == null || properties.GatewayAddresses.Count == 0)
                continue;

            // 查找IPv4地址
            foreach (var address in properties.UnicastAddresses)
            {
                if (address.Address.AddressFamily == AddressFamily.InterNetwork) // IPv4
                {
                    return address.Address.ToString();
                }
            }
        }

        throw new Exception("无法找到有效的本地以太网IPv4地址");
    }
}