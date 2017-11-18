using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Text;
using System.Threading;

namespace Vdro550App
{
    public partial class VdroForm : Form
    {
        public VdroForm()
        {
            InitializeComponent();
        }

        private PrivateFontCollection pfc = new PrivateFontCollection();

        private void VdroForm_Load(object sender, EventArgs e)
        {
            Stream fontStream = this.GetType().Assembly.GetManifestResourceStream("Vdro550App.Crysta.ttf");
            byte[] fontdata = new byte[fontStream.Length];

            fontStream.Read(fontdata, 0, (int)fontStream.Length);
            fontStream.Close();

            unsafe
            {
                fixed (byte* pFontData = fontdata)
                {
                    pfc.AddMemoryFont((System.IntPtr)pFontData, fontdata.Length);
                }
            }
            this.xDisplayLabel.Font = new System.Drawing.Font(pfc.Families[0], 48F, System.Drawing.FontStyle.Italic);
            this.yDisplayLabel.Font = new System.Drawing.Font(pfc.Families[0], 48F, System.Drawing.FontStyle.Italic);
            this.zDisplayLabel.Font = new System.Drawing.Font(pfc.Families[0], 48F, System.Drawing.FontStyle.Italic);

            foreach (string name in System.IO.Ports.SerialPort.GetPortNames())
            {
                this.serialPortComboBox.Items.Add(name);
            }

            this.serialPort.NewLine = "\r\n";
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        public Point mouse_offset;

        private void VdroForm_MouseDown(object sender, MouseEventArgs e)
        {
            mouse_offset = new Point(-e.X, -e.Y);
        }

        private void VdroForm_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Point mousePos = Control.MousePosition;
                mousePos.Offset(mouse_offset.X, mouse_offset.Y);
                Location = mousePos;
            }
        }

        private void Button_MouseMove(object sender, MouseEventArgs e)
        {
            Cursor.Current = Cursors.Hand;
        }

        private void keypadPress(string key)
        {
            if (this.serialPort.IsOpen)
                this.serialPort.WriteLine("keypad.press('" + key + "')");
        }

        private void MinimizeButton_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Minimized;
        }

        private void absIncrButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("absincr");
        }

        private Mutex serialMutex = new Mutex(false, "Serial");

        private System.IO.Ports.SerialDataReceivedEventHandler serialHandler;

        private void serialPortComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.serialMutex.WaitOne();
            try
            {
                this.displayTimer.Stop();
                this.serialPort.Close();
                this.serialPort.DataReceived -= serialHandler;
                this.serialPort.PortName = this.serialPortComboBox.SelectedItem.ToString();
                this.serialPort.Open();
            }
            catch
            {
                this.serialMutex.ReleaseMutex();
                MessageBox.Show("Unable to open serial port " + serialPort.PortName,
                    "Serial Port Error",
                    System.Windows.Forms.MessageBoxButtons.OK,
                    System.Windows.Forms.MessageBoxIcon.Error);
                return;
            }
            try
            {
                this.serialPort.ReadTimeout = 20;

                // Turn on automate mode
                this.serialPort.WriteLine("luash.automate(true)");

                // Disable trace output
                this.serialPort.WriteLine("trace.fatal()");

                // Start the polling timer to update the display
                serialHandler = new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);
                this.serialPort.DataReceived += serialHandler;
                this.displayTimer.Start();
            }
            catch
            {
                this.displayTimer.Stop();
                this.serialPort.Close();
                this.serialMutex.ReleaseMutex();
                MessageBox.Show("Unable to communicate with DRO-550",
                    "Communication Error",
                    System.Windows.Forms.MessageBoxButtons.OK,
                    System.Windows.Forms.MessageBoxIcon.Error);
                return;
            }
            this.serialMutex.ReleaseMutex();
        }

        private int txLineNum = 1;
        private int rxLineNum = 1;
        private bool menuOn = false;
        private const int numLines = 3;

        private void displayTimer_Tick(object sender, EventArgs e)
        {
            if (this.txLineNum <= numLines)
            {
                if (menuOn)
                    this.serialPort.WriteLine("print(display.menuget(" + this.txLineNum + "))");
                else
                    this.serialPort.WriteLine("print(display.readget(" + this.txLineNum + "))");
            }
            else if (this.txLineNum == numLines + 1)
            {
                this.serialPort.WriteLine("for idx,ind in ipairs(display.ind()) do if ind then io.write('1') else io.write('0') end end print('!')");
            }
            else if (this.txLineNum > numLines + 2)
            {
                this.serialPort.WriteLine("print(display.menustate())");
            }

            if (this.txLineNum > numLines + 2)
                this.txLineNum = 1;
            else
                this.txLineNum++;
        }

        private void SetXText(string text)
        {
            this.xDisplayLabel.Text = text.ToUpper();
        }

        private void SetYText(string text)
        {
            this.yDisplayLabel.Text = text.ToUpper();
        }

        private void SetZText(string text)
        {
            this.zDisplayLabel.Text = text.ToUpper();
        }

        private void SetInd(string text)
        {
            if (text.Length == 6 && text[5] == '!')
            {
                this.incrLabel.Visible = (text[0] == '1');
                this.mmLabel.Visible = (text[1] == '1');
                this.zeroLabel.Visible = (text[2] == '1');
                this.setLabel.Visible = (text[3] == '1');
                this.funcLabel.Visible = (text[4] == '1');
            }
        }

        private void MenuState(string text)
        {
            this.menuOn = (text == "true");
        }

        delegate void SetCallback(string text);

        private void serialPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string line;

            while (this.serialPort.BytesToRead > 0)
            {
                try
                {
                    this.serialMutex.WaitOne();
                    line = this.serialPort.ReadLine();
                    this.serialMutex.ReleaseMutex();
                }
                catch
                {
                    this.serialMutex.ReleaseMutex();
                    MessageBox.Show("Communication lost with DRO-550",
                        "Communication Lost",
                        System.Windows.Forms.MessageBoxButtons.OK,
                        System.Windows.Forms.MessageBoxIcon.Error);
                    return;
                }

                // Ignore error lines
                if (line.StartsWith("luash"))
                    return;

                // Safety check in case we get out of sync with the transmitter
                if (line.Length == 6 && line[5] == '!')
                    this.rxLineNum = 4;

                if (this.rxLineNum == 1)
                    this.Invoke(new SetCallback(SetXText), new object[] { line });
                else if (this.rxLineNum == 2)
                    this.Invoke(new SetCallback(SetYText), new object[] { line });
                else if (this.rxLineNum == 3)
                    this.Invoke(new SetCallback(SetZText), new object[] { line });
                else if (this.rxLineNum == 4)
                    this.Invoke(new SetCallback(SetInd), new object[] { line });
                else
                    this.Invoke(new SetCallback(MenuState), new object[] { line });

                if (this.rxLineNum == numLines + 2)
                    this.rxLineNum = 1;
                else
                    this.rxLineNum++;
            }
        }

        private void inchMmButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("mminch");
        }

        private void funcButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("func");
        }

        private void xPresetButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("preset1");
        }

        private void xZeroButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("zero1");
        }

        private void yPresetButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("preset2");
        }

        private void yZeroButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("zero2");
        }

        private void zPresetButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("preset3");
        }

        private void zZeroButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("zero3");
        }

        private void decimalButton_Click(object sender, EventArgs e)
        {
            this.keypadPress(".");
        }

        private void num0Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("0");
        }

        private void plusMinusButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("+-");
        }

        private void num1Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("1");
        }

        private void num2Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("2");
        }

        private void num3Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("3");
        }

        private void num4Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("4");
        }

        private void num5Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("5");
        }

        private void num6Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("6");
        }

        private void num7Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("7");
        }

        private void num8Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("8");
        }

        private void num9Button_Click(object sender, EventArgs e)
        {
            this.keypadPress("9");
        }

        private void enterButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("enter");
        }

        private void clearButton_Click(object sender, EventArgs e)
        {
            this.keypadPress("clear");
        }

        private void VdroForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.serialPort.IsOpen)
            {
                try
                {
                    this.serialMutex.WaitOne();

                    // Restore the trace level to info
                    this.serialPort.WriteLine("trace.info()");

                    // Turn off automate mode
                    this.serialPort.WriteLine("luash.automate(false)");
                }
                catch
                {
                }
                this.serialMutex.ReleaseMutex();
            }
        }
    }
}
