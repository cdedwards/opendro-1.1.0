namespace Vdro550App
{
    partial class VdroForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(VdroForm));
            this.xDisplayLabel = new System.Windows.Forms.Label();
            this.yDisplayLabel = new System.Windows.Forms.Label();
            this.zDisplayLabel = new System.Windows.Forms.Label();
            this.closeButton = new System.Windows.Forms.Button();
            this.minimizeButton = new System.Windows.Forms.Button();
            this.xPresetButton = new System.Windows.Forms.Button();
            this.yPresetButton = new System.Windows.Forms.Button();
            this.zPresetButton = new System.Windows.Forms.Button();
            this.zZeroButton = new System.Windows.Forms.Button();
            this.yZeroButton = new System.Windows.Forms.Button();
            this.xZeroButton = new System.Windows.Forms.Button();
            this.absIncrButton = new System.Windows.Forms.Button();
            this.num7Button = new System.Windows.Forms.Button();
            this.num4Button = new System.Windows.Forms.Button();
            this.num1Button = new System.Windows.Forms.Button();
            this.decimalButton = new System.Windows.Forms.Button();
            this.num0Button = new System.Windows.Forms.Button();
            this.num2Button = new System.Windows.Forms.Button();
            this.num5Button = new System.Windows.Forms.Button();
            this.num8Button = new System.Windows.Forms.Button();
            this.inchMmButton = new System.Windows.Forms.Button();
            this.plusMinusButton = new System.Windows.Forms.Button();
            this.num3Button = new System.Windows.Forms.Button();
            this.num6Button = new System.Windows.Forms.Button();
            this.num9Button = new System.Windows.Forms.Button();
            this.funcButton = new System.Windows.Forms.Button();
            this.clearButton = new System.Windows.Forms.Button();
            this.enterButton = new System.Windows.Forms.Button();
            this.serialPortComboBox = new System.Windows.Forms.ComboBox();
            this.incrLabel = new System.Windows.Forms.Label();
            this.mmLabel = new System.Windows.Forms.Label();
            this.zeroLabel = new System.Windows.Forms.Label();
            this.setLabel = new System.Windows.Forms.Label();
            this.funcLabel = new System.Windows.Forms.Label();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.displayTimer = new System.Windows.Forms.Timer(this.components);
            this.serialPortLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // xDisplayLabel
            // 
            this.xDisplayLabel.BackColor = System.Drawing.Color.Transparent;
            this.xDisplayLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 48F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.xDisplayLabel.ForeColor = System.Drawing.Color.Red;
            this.xDisplayLabel.Location = new System.Drawing.Point(70, 54);
            this.xDisplayLabel.Margin = new System.Windows.Forms.Padding(0);
            this.xDisplayLabel.Name = "xDisplayLabel";
            this.xDisplayLabel.Size = new System.Drawing.Size(246, 72);
            this.xDisplayLabel.TabIndex = 0;
            this.xDisplayLabel.Text = "SELECT";
            this.xDisplayLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.xDisplayLabel.UseCompatibleTextRendering = true;
            // 
            // yDisplayLabel
            // 
            this.yDisplayLabel.BackColor = System.Drawing.Color.Transparent;
            this.yDisplayLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 48F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.yDisplayLabel.ForeColor = System.Drawing.Color.Red;
            this.yDisplayLabel.Location = new System.Drawing.Point(70, 142);
            this.yDisplayLabel.Margin = new System.Windows.Forms.Padding(0);
            this.yDisplayLabel.Name = "yDisplayLabel";
            this.yDisplayLabel.Size = new System.Drawing.Size(246, 72);
            this.yDisplayLabel.TabIndex = 1;
            this.yDisplayLabel.Text = "SERIAL";
            this.yDisplayLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.yDisplayLabel.UseCompatibleTextRendering = true;
            // 
            // zDisplayLabel
            // 
            this.zDisplayLabel.BackColor = System.Drawing.Color.Transparent;
            this.zDisplayLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 48F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.zDisplayLabel.ForeColor = System.Drawing.Color.Red;
            this.zDisplayLabel.Location = new System.Drawing.Point(70, 230);
            this.zDisplayLabel.Margin = new System.Windows.Forms.Padding(0);
            this.zDisplayLabel.Name = "zDisplayLabel";
            this.zDisplayLabel.Size = new System.Drawing.Size(246, 72);
            this.zDisplayLabel.TabIndex = 2;
            this.zDisplayLabel.Text = "PORT";
            this.zDisplayLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.zDisplayLabel.UseCompatibleTextRendering = true;
            // 
            // closeButton
            // 
            this.closeButton.BackColor = System.Drawing.Color.Transparent;
            this.closeButton.FlatAppearance.BorderSize = 0;
            this.closeButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.closeButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.closeButton.Location = new System.Drawing.Point(646, 0);
            this.closeButton.Margin = new System.Windows.Forms.Padding(0);
            this.closeButton.Name = "closeButton";
            this.closeButton.Size = new System.Drawing.Size(23, 23);
            this.closeButton.TabIndex = 3;
            this.closeButton.UseVisualStyleBackColor = false;
            this.closeButton.Click += new System.EventHandler(this.CloseButton_Click);
            this.closeButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // minimizeButton
            // 
            this.minimizeButton.BackColor = System.Drawing.Color.Transparent;
            this.minimizeButton.FlatAppearance.BorderSize = 0;
            this.minimizeButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.minimizeButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.minimizeButton.Location = new System.Drawing.Point(617, 0);
            this.minimizeButton.Margin = new System.Windows.Forms.Padding(0);
            this.minimizeButton.Name = "minimizeButton";
            this.minimizeButton.Size = new System.Drawing.Size(23, 23);
            this.minimizeButton.TabIndex = 4;
            this.minimizeButton.UseVisualStyleBackColor = false;
            this.minimizeButton.Click += new System.EventHandler(this.MinimizeButton_Click);
            this.minimizeButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // xPresetButton
            // 
            this.xPresetButton.BackColor = System.Drawing.Color.Transparent;
            this.xPresetButton.FlatAppearance.BorderSize = 0;
            this.xPresetButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.xPresetButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.xPresetButton.Location = new System.Drawing.Point(327, 63);
            this.xPresetButton.Margin = new System.Windows.Forms.Padding(0);
            this.xPresetButton.Name = "xPresetButton";
            this.xPresetButton.Size = new System.Drawing.Size(46, 46);
            this.xPresetButton.TabIndex = 5;
            this.xPresetButton.UseVisualStyleBackColor = false;
            this.xPresetButton.Click += new System.EventHandler(this.xPresetButton_Click);
            this.xPresetButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // yPresetButton
            // 
            this.yPresetButton.BackColor = System.Drawing.Color.Transparent;
            this.yPresetButton.FlatAppearance.BorderSize = 0;
            this.yPresetButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.yPresetButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.yPresetButton.Location = new System.Drawing.Point(327, 151);
            this.yPresetButton.Margin = new System.Windows.Forms.Padding(0);
            this.yPresetButton.Name = "yPresetButton";
            this.yPresetButton.Size = new System.Drawing.Size(46, 46);
            this.yPresetButton.TabIndex = 6;
            this.yPresetButton.UseVisualStyleBackColor = false;
            this.yPresetButton.Click += new System.EventHandler(this.yPresetButton_Click);
            this.yPresetButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // zPresetButton
            // 
            this.zPresetButton.BackColor = System.Drawing.Color.Transparent;
            this.zPresetButton.FlatAppearance.BorderSize = 0;
            this.zPresetButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.zPresetButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.zPresetButton.Location = new System.Drawing.Point(327, 239);
            this.zPresetButton.Margin = new System.Windows.Forms.Padding(0);
            this.zPresetButton.Name = "zPresetButton";
            this.zPresetButton.Size = new System.Drawing.Size(46, 46);
            this.zPresetButton.TabIndex = 7;
            this.zPresetButton.UseVisualStyleBackColor = false;
            this.zPresetButton.Click += new System.EventHandler(this.zPresetButton_Click);
            this.zPresetButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // zZeroButton
            // 
            this.zZeroButton.BackColor = System.Drawing.Color.Transparent;
            this.zZeroButton.FlatAppearance.BorderSize = 0;
            this.zZeroButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.zZeroButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.zZeroButton.Location = new System.Drawing.Point(384, 239);
            this.zZeroButton.Margin = new System.Windows.Forms.Padding(0);
            this.zZeroButton.Name = "zZeroButton";
            this.zZeroButton.Size = new System.Drawing.Size(46, 46);
            this.zZeroButton.TabIndex = 10;
            this.zZeroButton.UseVisualStyleBackColor = false;
            this.zZeroButton.Click += new System.EventHandler(this.zZeroButton_Click);
            this.zZeroButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // yZeroButton
            // 
            this.yZeroButton.BackColor = System.Drawing.Color.Transparent;
            this.yZeroButton.FlatAppearance.BorderSize = 0;
            this.yZeroButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.yZeroButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.yZeroButton.Location = new System.Drawing.Point(384, 151);
            this.yZeroButton.Margin = new System.Windows.Forms.Padding(0);
            this.yZeroButton.Name = "yZeroButton";
            this.yZeroButton.Size = new System.Drawing.Size(46, 46);
            this.yZeroButton.TabIndex = 9;
            this.yZeroButton.UseVisualStyleBackColor = false;
            this.yZeroButton.Click += new System.EventHandler(this.yZeroButton_Click);
            this.yZeroButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // xZeroButton
            // 
            this.xZeroButton.BackColor = System.Drawing.Color.Transparent;
            this.xZeroButton.FlatAppearance.BorderSize = 0;
            this.xZeroButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.xZeroButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.xZeroButton.Location = new System.Drawing.Point(384, 63);
            this.xZeroButton.Margin = new System.Windows.Forms.Padding(0);
            this.xZeroButton.Name = "xZeroButton";
            this.xZeroButton.Size = new System.Drawing.Size(46, 46);
            this.xZeroButton.TabIndex = 8;
            this.xZeroButton.UseVisualStyleBackColor = false;
            this.xZeroButton.Click += new System.EventHandler(this.xZeroButton_Click);
            this.xZeroButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // absIncrButton
            // 
            this.absIncrButton.BackColor = System.Drawing.Color.Transparent;
            this.absIncrButton.FlatAppearance.BorderSize = 0;
            this.absIncrButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.absIncrButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.absIncrButton.Location = new System.Drawing.Point(447, 38);
            this.absIncrButton.Margin = new System.Windows.Forms.Padding(0);
            this.absIncrButton.Name = "absIncrButton";
            this.absIncrButton.Size = new System.Drawing.Size(46, 46);
            this.absIncrButton.TabIndex = 11;
            this.absIncrButton.UseVisualStyleBackColor = false;
            this.absIncrButton.Click += new System.EventHandler(this.absIncrButton_Click);
            this.absIncrButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num7Button
            // 
            this.num7Button.BackColor = System.Drawing.Color.Transparent;
            this.num7Button.FlatAppearance.BorderSize = 0;
            this.num7Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num7Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num7Button.Location = new System.Drawing.Point(447, 95);
            this.num7Button.Margin = new System.Windows.Forms.Padding(0);
            this.num7Button.Name = "num7Button";
            this.num7Button.Size = new System.Drawing.Size(46, 46);
            this.num7Button.TabIndex = 12;
            this.num7Button.UseVisualStyleBackColor = false;
            this.num7Button.Click += new System.EventHandler(this.num7Button_Click);
            this.num7Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num4Button
            // 
            this.num4Button.BackColor = System.Drawing.Color.Transparent;
            this.num4Button.FlatAppearance.BorderSize = 0;
            this.num4Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num4Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num4Button.Location = new System.Drawing.Point(447, 151);
            this.num4Button.Margin = new System.Windows.Forms.Padding(0);
            this.num4Button.Name = "num4Button";
            this.num4Button.Size = new System.Drawing.Size(46, 46);
            this.num4Button.TabIndex = 13;
            this.num4Button.UseVisualStyleBackColor = false;
            this.num4Button.Click += new System.EventHandler(this.num4Button_Click);
            this.num4Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num1Button
            // 
            this.num1Button.BackColor = System.Drawing.Color.Transparent;
            this.num1Button.FlatAppearance.BorderSize = 0;
            this.num1Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num1Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num1Button.Location = new System.Drawing.Point(447, 207);
            this.num1Button.Margin = new System.Windows.Forms.Padding(0);
            this.num1Button.Name = "num1Button";
            this.num1Button.Size = new System.Drawing.Size(46, 46);
            this.num1Button.TabIndex = 14;
            this.num1Button.UseVisualStyleBackColor = false;
            this.num1Button.Click += new System.EventHandler(this.num1Button_Click);
            this.num1Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // decimalButton
            // 
            this.decimalButton.BackColor = System.Drawing.Color.Transparent;
            this.decimalButton.FlatAppearance.BorderSize = 0;
            this.decimalButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.decimalButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.decimalButton.Location = new System.Drawing.Point(447, 263);
            this.decimalButton.Margin = new System.Windows.Forms.Padding(0);
            this.decimalButton.Name = "decimalButton";
            this.decimalButton.Size = new System.Drawing.Size(46, 46);
            this.decimalButton.TabIndex = 15;
            this.decimalButton.UseVisualStyleBackColor = false;
            this.decimalButton.Click += new System.EventHandler(this.decimalButton_Click);
            this.decimalButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num0Button
            // 
            this.num0Button.BackColor = System.Drawing.Color.Transparent;
            this.num0Button.FlatAppearance.BorderSize = 0;
            this.num0Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num0Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num0Button.Location = new System.Drawing.Point(503, 263);
            this.num0Button.Margin = new System.Windows.Forms.Padding(0);
            this.num0Button.Name = "num0Button";
            this.num0Button.Size = new System.Drawing.Size(46, 46);
            this.num0Button.TabIndex = 20;
            this.num0Button.UseVisualStyleBackColor = false;
            this.num0Button.Click += new System.EventHandler(this.num0Button_Click);
            this.num0Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num2Button
            // 
            this.num2Button.BackColor = System.Drawing.Color.Transparent;
            this.num2Button.FlatAppearance.BorderSize = 0;
            this.num2Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num2Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num2Button.Location = new System.Drawing.Point(503, 207);
            this.num2Button.Margin = new System.Windows.Forms.Padding(0);
            this.num2Button.Name = "num2Button";
            this.num2Button.Size = new System.Drawing.Size(46, 46);
            this.num2Button.TabIndex = 19;
            this.num2Button.UseVisualStyleBackColor = false;
            this.num2Button.Click += new System.EventHandler(this.num2Button_Click);
            this.num2Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num5Button
            // 
            this.num5Button.BackColor = System.Drawing.Color.Transparent;
            this.num5Button.FlatAppearance.BorderSize = 0;
            this.num5Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num5Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num5Button.Location = new System.Drawing.Point(503, 151);
            this.num5Button.Margin = new System.Windows.Forms.Padding(0);
            this.num5Button.Name = "num5Button";
            this.num5Button.Size = new System.Drawing.Size(46, 46);
            this.num5Button.TabIndex = 18;
            this.num5Button.UseVisualStyleBackColor = false;
            this.num5Button.Click += new System.EventHandler(this.num5Button_Click);
            this.num5Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num8Button
            // 
            this.num8Button.BackColor = System.Drawing.Color.Transparent;
            this.num8Button.FlatAppearance.BorderSize = 0;
            this.num8Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num8Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num8Button.Location = new System.Drawing.Point(503, 95);
            this.num8Button.Margin = new System.Windows.Forms.Padding(0);
            this.num8Button.Name = "num8Button";
            this.num8Button.Size = new System.Drawing.Size(46, 46);
            this.num8Button.TabIndex = 17;
            this.num8Button.UseVisualStyleBackColor = false;
            this.num8Button.Click += new System.EventHandler(this.num8Button_Click);
            this.num8Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // inchMmButton
            // 
            this.inchMmButton.BackColor = System.Drawing.Color.Transparent;
            this.inchMmButton.FlatAppearance.BorderSize = 0;
            this.inchMmButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.inchMmButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.inchMmButton.Location = new System.Drawing.Point(503, 38);
            this.inchMmButton.Margin = new System.Windows.Forms.Padding(0);
            this.inchMmButton.Name = "inchMmButton";
            this.inchMmButton.Size = new System.Drawing.Size(46, 46);
            this.inchMmButton.TabIndex = 16;
            this.inchMmButton.UseVisualStyleBackColor = false;
            this.inchMmButton.Click += new System.EventHandler(this.inchMmButton_Click);
            this.inchMmButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // plusMinusButton
            // 
            this.plusMinusButton.BackColor = System.Drawing.Color.Transparent;
            this.plusMinusButton.FlatAppearance.BorderSize = 0;
            this.plusMinusButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.plusMinusButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.plusMinusButton.Location = new System.Drawing.Point(559, 263);
            this.plusMinusButton.Margin = new System.Windows.Forms.Padding(0);
            this.plusMinusButton.Name = "plusMinusButton";
            this.plusMinusButton.Size = new System.Drawing.Size(46, 46);
            this.plusMinusButton.TabIndex = 25;
            this.plusMinusButton.UseVisualStyleBackColor = false;
            this.plusMinusButton.Click += new System.EventHandler(this.plusMinusButton_Click);
            this.plusMinusButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num3Button
            // 
            this.num3Button.BackColor = System.Drawing.Color.Transparent;
            this.num3Button.FlatAppearance.BorderSize = 0;
            this.num3Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num3Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num3Button.Location = new System.Drawing.Point(559, 207);
            this.num3Button.Margin = new System.Windows.Forms.Padding(0);
            this.num3Button.Name = "num3Button";
            this.num3Button.Size = new System.Drawing.Size(46, 46);
            this.num3Button.TabIndex = 24;
            this.num3Button.UseVisualStyleBackColor = false;
            this.num3Button.Click += new System.EventHandler(this.num3Button_Click);
            this.num3Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num6Button
            // 
            this.num6Button.BackColor = System.Drawing.Color.Transparent;
            this.num6Button.FlatAppearance.BorderSize = 0;
            this.num6Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num6Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num6Button.Location = new System.Drawing.Point(559, 151);
            this.num6Button.Margin = new System.Windows.Forms.Padding(0);
            this.num6Button.Name = "num6Button";
            this.num6Button.Size = new System.Drawing.Size(46, 46);
            this.num6Button.TabIndex = 23;
            this.num6Button.UseVisualStyleBackColor = false;
            this.num6Button.Click += new System.EventHandler(this.num6Button_Click);
            this.num6Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // num9Button
            // 
            this.num9Button.BackColor = System.Drawing.Color.Transparent;
            this.num9Button.FlatAppearance.BorderSize = 0;
            this.num9Button.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.num9Button.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.num9Button.Location = new System.Drawing.Point(559, 95);
            this.num9Button.Margin = new System.Windows.Forms.Padding(0);
            this.num9Button.Name = "num9Button";
            this.num9Button.Size = new System.Drawing.Size(46, 46);
            this.num9Button.TabIndex = 22;
            this.num9Button.UseVisualStyleBackColor = false;
            this.num9Button.Click += new System.EventHandler(this.num9Button_Click);
            this.num9Button.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // funcButton
            // 
            this.funcButton.BackColor = System.Drawing.Color.Transparent;
            this.funcButton.FlatAppearance.BorderSize = 0;
            this.funcButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.funcButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.funcButton.Location = new System.Drawing.Point(559, 38);
            this.funcButton.Margin = new System.Windows.Forms.Padding(0);
            this.funcButton.Name = "funcButton";
            this.funcButton.Size = new System.Drawing.Size(46, 46);
            this.funcButton.TabIndex = 21;
            this.funcButton.UseVisualStyleBackColor = false;
            this.funcButton.Click += new System.EventHandler(this.funcButton_Click);
            this.funcButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // clearButton
            // 
            this.clearButton.BackColor = System.Drawing.Color.Transparent;
            this.clearButton.FlatAppearance.BorderSize = 0;
            this.clearButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.clearButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.clearButton.Location = new System.Drawing.Point(615, 111);
            this.clearButton.Margin = new System.Windows.Forms.Padding(0);
            this.clearButton.Name = "clearButton";
            this.clearButton.Size = new System.Drawing.Size(46, 46);
            this.clearButton.TabIndex = 26;
            this.clearButton.UseVisualStyleBackColor = false;
            this.clearButton.Click += new System.EventHandler(this.clearButton_Click);
            this.clearButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // enterButton
            // 
            this.enterButton.BackColor = System.Drawing.Color.Transparent;
            this.enterButton.FlatAppearance.BorderSize = 0;
            this.enterButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.enterButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.enterButton.Location = new System.Drawing.Point(615, 191);
            this.enterButton.Margin = new System.Windows.Forms.Padding(0);
            this.enterButton.Name = "enterButton";
            this.enterButton.Size = new System.Drawing.Size(46, 46);
            this.enterButton.TabIndex = 27;
            this.enterButton.UseVisualStyleBackColor = false;
            this.enterButton.Click += new System.EventHandler(this.enterButton_Click);
            this.enterButton.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Button_MouseMove);
            // 
            // serialPortComboBox
            // 
            this.serialPortComboBox.BackColor = System.Drawing.Color.SteelBlue;
            this.serialPortComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.serialPortComboBox.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold);
            this.serialPortComboBox.ForeColor = System.Drawing.Color.White;
            this.serialPortComboBox.FormattingEnabled = true;
            this.serialPortComboBox.Location = new System.Drawing.Point(327, 295);
            this.serialPortComboBox.Name = "serialPortComboBox";
            this.serialPortComboBox.Size = new System.Drawing.Size(70, 24);
            this.serialPortComboBox.TabIndex = 28;
            this.serialPortComboBox.SelectedIndexChanged += new System.EventHandler(this.serialPortComboBox_SelectedIndexChanged);
            // 
            // incrLabel
            // 
            this.incrLabel.BackColor = System.Drawing.Color.Transparent;
            this.incrLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.incrLabel.Image = ((System.Drawing.Image)(resources.GetObject("incrLabel.Image")));
            this.incrLabel.Location = new System.Drawing.Point(108, 38);
            this.incrLabel.Margin = new System.Windows.Forms.Padding(0);
            this.incrLabel.Name = "incrLabel";
            this.incrLabel.Size = new System.Drawing.Size(13, 13);
            this.incrLabel.TabIndex = 29;
            this.incrLabel.Visible = false;
            // 
            // mmLabel
            // 
            this.mmLabel.BackColor = System.Drawing.Color.Transparent;
            this.mmLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.mmLabel.Image = ((System.Drawing.Image)(resources.GetObject("mmLabel.Image")));
            this.mmLabel.Location = new System.Drawing.Point(148, 38);
            this.mmLabel.Margin = new System.Windows.Forms.Padding(0);
            this.mmLabel.Name = "mmLabel";
            this.mmLabel.Size = new System.Drawing.Size(13, 13);
            this.mmLabel.TabIndex = 30;
            this.mmLabel.Visible = false;
            // 
            // zeroLabel
            // 
            this.zeroLabel.BackColor = System.Drawing.Color.Transparent;
            this.zeroLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.zeroLabel.Image = ((System.Drawing.Image)(resources.GetObject("zeroLabel.Image")));
            this.zeroLabel.Location = new System.Drawing.Point(188, 38);
            this.zeroLabel.Margin = new System.Windows.Forms.Padding(0);
            this.zeroLabel.Name = "zeroLabel";
            this.zeroLabel.Size = new System.Drawing.Size(13, 13);
            this.zeroLabel.TabIndex = 31;
            this.zeroLabel.Visible = false;
            // 
            // setLabel
            // 
            this.setLabel.BackColor = System.Drawing.Color.Transparent;
            this.setLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.setLabel.Image = ((System.Drawing.Image)(resources.GetObject("setLabel.Image")));
            this.setLabel.Location = new System.Drawing.Point(228, 38);
            this.setLabel.Margin = new System.Windows.Forms.Padding(0);
            this.setLabel.Name = "setLabel";
            this.setLabel.Size = new System.Drawing.Size(13, 13);
            this.setLabel.TabIndex = 32;
            this.setLabel.Visible = false;
            // 
            // funcLabel
            // 
            this.funcLabel.BackColor = System.Drawing.Color.Transparent;
            this.funcLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.funcLabel.Image = ((System.Drawing.Image)(resources.GetObject("funcLabel.Image")));
            this.funcLabel.Location = new System.Drawing.Point(268, 38);
            this.funcLabel.Margin = new System.Windows.Forms.Padding(0);
            this.funcLabel.Name = "funcLabel";
            this.funcLabel.Size = new System.Drawing.Size(13, 13);
            this.funcLabel.TabIndex = 33;
            this.funcLabel.Visible = false;
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 57600;
            // 
            // displayTimer
            // 
            this.displayTimer.Interval = 25;
            this.displayTimer.Tick += new System.EventHandler(this.displayTimer_Tick);
            // 
            // serialPortLabel
            // 
            this.serialPortLabel.AutoSize = true;
            this.serialPortLabel.BackColor = System.Drawing.Color.Transparent;
            this.serialPortLabel.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Bold);
            this.serialPortLabel.ForeColor = System.Drawing.Color.White;
            this.serialPortLabel.Location = new System.Drawing.Point(238, 300);
            this.serialPortLabel.Name = "serialPortLabel";
            this.serialPortLabel.Size = new System.Drawing.Size(85, 16);
            this.serialPortLabel.TabIndex = 34;
            this.serialPortLabel.Text = "Serial Port:";
            // 
            // VdroForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(693, 346);
            this.Controls.Add(this.serialPortLabel);
            this.Controls.Add(this.funcLabel);
            this.Controls.Add(this.setLabel);
            this.Controls.Add(this.zeroLabel);
            this.Controls.Add(this.mmLabel);
            this.Controls.Add(this.incrLabel);
            this.Controls.Add(this.serialPortComboBox);
            this.Controls.Add(this.enterButton);
            this.Controls.Add(this.clearButton);
            this.Controls.Add(this.plusMinusButton);
            this.Controls.Add(this.num3Button);
            this.Controls.Add(this.num6Button);
            this.Controls.Add(this.num9Button);
            this.Controls.Add(this.funcButton);
            this.Controls.Add(this.num0Button);
            this.Controls.Add(this.num2Button);
            this.Controls.Add(this.num5Button);
            this.Controls.Add(this.num8Button);
            this.Controls.Add(this.inchMmButton);
            this.Controls.Add(this.decimalButton);
            this.Controls.Add(this.num1Button);
            this.Controls.Add(this.num4Button);
            this.Controls.Add(this.num7Button);
            this.Controls.Add(this.absIncrButton);
            this.Controls.Add(this.zZeroButton);
            this.Controls.Add(this.yZeroButton);
            this.Controls.Add(this.xZeroButton);
            this.Controls.Add(this.zPresetButton);
            this.Controls.Add(this.yPresetButton);
            this.Controls.Add(this.xPresetButton);
            this.Controls.Add(this.minimizeButton);
            this.Controls.Add(this.closeButton);
            this.Controls.Add(this.zDisplayLabel);
            this.Controls.Add(this.yDisplayLabel);
            this.Controls.Add(this.xDisplayLabel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "VdroForm";
            this.Text = "VDRO550";
            this.TransparencyKey = System.Drawing.Color.Fuchsia;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.VdroForm_FormClosing);
            this.Load += new System.EventHandler(this.VdroForm_Load);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.VdroForm_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.VdroForm_MouseMove);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label xDisplayLabel;
        private System.Windows.Forms.Label yDisplayLabel;
        private System.Windows.Forms.Label zDisplayLabel;
        private System.Windows.Forms.Button closeButton;
        private System.Windows.Forms.Button minimizeButton;
        private System.Windows.Forms.Button xPresetButton;
        private System.Windows.Forms.Button yPresetButton;
        private System.Windows.Forms.Button zPresetButton;
        private System.Windows.Forms.Button zZeroButton;
        private System.Windows.Forms.Button yZeroButton;
        private System.Windows.Forms.Button xZeroButton;
        private System.Windows.Forms.Button absIncrButton;
        private System.Windows.Forms.Button num7Button;
        private System.Windows.Forms.Button num4Button;
        private System.Windows.Forms.Button num1Button;
        private System.Windows.Forms.Button decimalButton;
        private System.Windows.Forms.Button num0Button;
        private System.Windows.Forms.Button num2Button;
        private System.Windows.Forms.Button num5Button;
        private System.Windows.Forms.Button num8Button;
        private System.Windows.Forms.Button inchMmButton;
        private System.Windows.Forms.Button plusMinusButton;
        private System.Windows.Forms.Button num3Button;
        private System.Windows.Forms.Button num6Button;
        private System.Windows.Forms.Button num9Button;
        private System.Windows.Forms.Button funcButton;
        private System.Windows.Forms.Button clearButton;
        private System.Windows.Forms.Button enterButton;
        private System.Windows.Forms.ComboBox serialPortComboBox;
        private System.Windows.Forms.Label incrLabel;
        private System.Windows.Forms.Label mmLabel;
        private System.Windows.Forms.Label zeroLabel;
        private System.Windows.Forms.Label setLabel;
        private System.Windows.Forms.Label funcLabel;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.Timer displayTimer;
        private System.Windows.Forms.Label serialPortLabel;
    }
}

