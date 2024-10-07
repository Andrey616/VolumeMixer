using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Threading.Tasks;

namespace ArduinoSerialReader
{
    class Program
    {
        static async Task Main(string[] args)
        {
            string portName = "COM13";
            int baudRate = 9600;

            bool connected = false;
            Process pythonProcess = null;
            StreamWriter pythonInput = null;
            StreamReader pythonOutput = null;

            try
            {
                // Настройка процесса Python
                ProcessStartInfo psi = new ProcessStartInfo
                {
                    FileName = "python", // Убедись, что python доступен в PATH
                    Arguments = "Volum.py",
                    RedirectStandardInput = true,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    UseShellExecute = false,
                    CreateNoWindow = true
                };

                pythonProcess = new Process { StartInfo = psi };
                pythonProcess.Start();

                pythonInput = pythonProcess.StandardInput;
                pythonOutput = pythonProcess.StandardOutput;

                // Асинхронное чтение ошибок Python
                pythonProcess.ErrorDataReceived += (sender, e) =>
                {
                    if (!string.IsNullOrEmpty(e.Data))
                    {
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.WriteLine($"Python Error: {e.Data}");
                        Console.ResetColor();
                    }
                };
                pythonProcess.BeginErrorReadLine();

                connected = true;
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine("Подключение к Arduino успешно установлено.");
                Console.WriteLine("Полученные данные от Arduino...");
                Console.ResetColor();

                using (SerialPort serialPort = new SerialPort(portName, baudRate))
                {
                    serialPort.Open();

                    while (true)
                    {
                        string data = serialPort.ReadLine();
                        Console.ForegroundColor = ConsoleColor.Green;
                        Console.SetCursorPosition(0, Console.CursorTop);
                        Console.Write(new string(' ', Console.WindowWidth - 1));
                        Console.SetCursorPosition(0, Console.CursorTop);
                        Console.Write(data);

                        // Отправка данных в Python
                        await pythonInput.WriteLineAsync(data);
                        await pythonInput.FlushAsync();





                    }
                }
            }
            catch (Exception ex)
            {
                Console.Clear();
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"Ошибка подключения: {ex.Message}");
                Console.WriteLine($"Повторная попытка подключения...");
                System.Threading.Thread.Sleep(1000);
                Console.Clear();
                // Можно реализовать повторную попытку подключения здесь
            }
            finally
            {
                if (pythonProcess != null && !pythonProcess.HasExited)
                {
                    pythonProcess.Kill();
                }
            }

        }
    }
}

