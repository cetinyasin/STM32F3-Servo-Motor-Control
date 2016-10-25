using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports; // Eklenmek zorunda.
using System.Threading;

namespace Test_SerialPort
{
    public partial class Form1 : Form
    {
        byte[] data = new byte[4];
        int checksum;
        //0.byte ulaşacak ID
        //1. byte Data tipi 0xA ise servoyu tam tur dönder. 0xB ise bir sonraki byte açı değeri
        //son byte checksum
        public Form1()
        {
            InitializeComponent();

            GetPortNames(); // Her başlangıçta eklesin.
            GetBaudRates();
            gB_pos.Enabled = false;
        }
        void GetPortNames() // available ports
        {
            string[] ports = SerialPort.GetPortNames();
            cb_Ports.Items.AddRange(ports);
        }

        void GetBaudRates()
        {
            cb_BRates.Items.Add("9600");
            cb_BRates.Items.Add("14400");
            cb_BRates.Items.Add("19200");
            cb_BRates.Items.Add("38400");
            cb_BRates.Items.Add("115200");
        }

        private void btn_Open_Click(object sender, EventArgs e)
        {
            try
            {
                if(cb_Ports.Text=="" || cb_BRates.Text=="")
                {
                    MessageBox.Show("Gerekli seçimler yapılmalıdır.");
                }
                else
                {
                    serialPort1.PortName = cb_Ports.Text;
                    serialPort1.BaudRate = int.Parse(cb_BRates.Text);
                    serialPort1.Open();
                    progressBar1.Value = 100;
                    MessageBox.Show("Bağlantı başarılı bir şekilde kuruldu.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btn_Close_Click(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen)
            {
                serialPort1.Close();
                progressBar1.Value = 0;
                MessageBox.Show("Bağlantı başarılı bir şekilde sonlandırıldı.");
            }
            else
            {
                MessageBox.Show("Kapatılacak bir bağlantı yok.");
            }
        }

        private void btn_Send_Click(object sender, EventArgs e)
        {
            ServoKomut(0x0B, Convert.ToByte(tb_Send.Text));
            trackBar1.Value = int.Parse(tb_Send.Text);
        }

        byte[] datam;
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (!(serialPort1.IsOpen))
            {
                serialPort1.Open();
            }
            Thread.Sleep(10);
            datam = new byte[serialPort1.BytesToRead];
            serialPort1.Read(datam, 0, serialPort1.BytesToRead);
            Invoke(new EventHandler(DisplayText));

        }
        private void DisplayText(object sender, EventArgs e)
        {
            string mystring = Encoding.Default.GetString(datam);
            lb_Received.Items.Add(mystring);
            lb_Received.SelectedIndex = lb_Received.Items.Count - 1;
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            ServoKomut(0x0B,Convert.ToByte(trackBar1.Value));
        }

        private void btn_Temizle_Click(object sender, EventArgs e)
        {
            lb_Received.Items.Clear();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (radioButton1.Checked)   // Pot Kontrol
            {
                gB_pos.Enabled = false;
                ServoKomut(0x0A, 0);
            }
            else if(radioButton2.Checked)
            {
                gB_pos.Enabled = true;
                ServoKomut(0x0B, Convert.ToByte(trackBar1.Value));
            }
        }

        void ServoKomut(byte konum, byte aci)
        {
            data[0] = 0x01; // stm32f3
            data[1] = konum; ;
            data[2] = aci;
            for (int i = 0; i < 3; i++)
            {
                checksum = checksum ^ data[i];
            }
            data[3] = Convert.ToByte(checksum);
            serialPort1.Write(data, 0, data.Length);
            checksum = 0;
        }
    }
}
