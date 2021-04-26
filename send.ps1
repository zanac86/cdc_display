$port= new-Object System.IO.Ports.SerialPort COM11,9600,None,8,One; $port.open(); $port.WriteLine("Have a nice day!\r\n"); $port.Close()
