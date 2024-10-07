using System;
using System.Diagnostics;
using System.IO.Ports;
using IronPython.Hosting;
using Microsoft.Scripting.Hosting;
using static IronPython.Modules._ast;

namespace ArduinoSerialReader
{
    class Program
    {
        static void Main(string[] args)
        {
            {

                
                string portName = "COM13"; 
                int baudRate = 9600;

                bool connected = false;

                while (!connected)
                {
                    try
                    {
                        using (SerialPort serialPort = new SerialPort(portName, baudRate))
                        {
                            serialPort.Open();

                            Console.ForegroundColor = ConsoleColor.Green;
                            Console.WriteLine("Подключение к Arduino успешно установлено.");
                            Console.WriteLine("Полученые данных от Arduino...");

                            while (true)
                            {
                                string data = serialPort.ReadLine();
                                Console.SetCursorPosition(0, Console.CursorTop);
                                Console.Write(new string(' ', Console.WindowWidth - 1));
                                Console.SetCursorPosition(0, Console.CursorTop);
                                Console.Write(data);

                                ScriptEngine engine = Python.CreateEngine();
                                
                                try
                                {
                                    
                                    ScriptScope val = engine.CreateScope();
                                    /* 100|100|100|100|100|100|100|100 */
                                    engine.ExecuteFile("Volum.py", val);
                                    dynamic pow = val.GetVariable("main");
                                    dynamic rez = pow(data);
                                    //Console.WriteLine(rez);
                                    
                                    //engine.Execute("print('hello, world')");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine($"Ошибка: {ex.Message}");
                                }
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.Clear();
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.WriteLine($"Ошибка подключения: {ex.Message}");
                        Console.WriteLine($"Повторная попытка подключения...");
                        System.Threading.Thread.Sleep(100);
                        Console.Clear();


                    }
                }
            }
        }
    }
}
