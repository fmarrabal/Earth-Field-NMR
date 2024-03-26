namespace EFNMR
{
    partial class Form1
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.SpectrometerParametersGroupBox = new System.Windows.Forms.GroupBox();
            this.responseLabel = new System.Windows.Forms.Label();
            this.setBackGroundCheckBox = new System.Windows.Forms.CheckBox();
            this.scansLabel = new System.Windows.Forms.Label();
            this.informationLabel = new System.Windows.Forms.Label();
            this.adquisitionTimeTextBox = new System.Windows.Forms.TextBox();
            this.adquisitonLabel = new System.Windows.Forms.Label();
            this.adquisitionProgressLabel = new System.Windows.Forms.Label();
            this.adquisitionProgressBar = new System.Windows.Forms.ProgressBar();
            this.fftFIDCheckBox = new System.Windows.Forms.CheckBox();
            this.button1 = new System.Windows.Forms.Button();
            this.pulsePowerTextBox = new System.Windows.Forms.TextBox();
            this.recoveryTimeTextBox = new System.Windows.Forms.TextBox();
            this.triggerSourceInfo = new System.Windows.Forms.Label();
            this.triggerSourceInfoAsterisk = new System.Windows.Forms.Label();
            this.triggerSourceTextBox = new System.Windows.Forms.TextBox();
            this.slopeGroupBox = new System.Windows.Forms.GroupBox();
            this.slopeRisingButton = new System.Windows.Forms.RadioButton();
            this.slopeFallingButton = new System.Windows.Forms.RadioButton();
            this.triggerSourceLabel = new System.Windows.Forms.Label();
            this.triggerLevelLabel = new System.Windows.Forms.Label();
            this.hysteresisLabel = new System.Windows.Forms.Label();
            this.channelParametersGroupBox = new System.Windows.Forms.GroupBox();
            this.physicalChannelComboBox = new System.Windows.Forms.ComboBox();
            this.minimumValueNumeric = new System.Windows.Forms.NumericUpDown();
            this.maximumValueNumeric = new System.Windows.Forms.NumericUpDown();
            this.maximumLabel = new System.Windows.Forms.Label();
            this.minimumLabel = new System.Windows.Forms.Label();
            this.physicalChannelLabel = new System.Windows.Forms.Label();
            this.timingParametersGroupBox = new System.Windows.Forms.GroupBox();
            this.rateLabel = new System.Windows.Forms.Label();
            this.samplesLabel = new System.Windows.Forms.Label();
            this.samplesPerChannelNumeric = new System.Windows.Forms.NumericUpDown();
            this.rateNumeric = new System.Windows.Forms.NumericUpDown();
            this.mainPlotScottPlot = new ScottPlot.FormsPlot();
            this.adquisitionTimer = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SpectrometerParametersGroupBox.SuspendLayout();
            this.slopeGroupBox.SuspendLayout();
            this.channelParametersGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.minimumValueNumeric)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.maximumValueNumeric)).BeginInit();
            this.timingParametersGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.samplesPerChannelNumeric)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.rateNumeric)).BeginInit();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.SpectrometerParametersGroupBox);
            this.splitContainer1.Panel1.Controls.Add(this.channelParametersGroupBox);
            this.splitContainer1.Panel1.Controls.Add(this.timingParametersGroupBox);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.mainPlotScottPlot);
            this.splitContainer1.Size = new System.Drawing.Size(959, 593);
            this.splitContainer1.SplitterDistance = 259;
            this.splitContainer1.TabIndex = 1;
            // 
            // SpectrometerParametersGroupBox
            // 
            this.SpectrometerParametersGroupBox.Controls.Add(this.responseLabel);
            this.SpectrometerParametersGroupBox.Controls.Add(this.setBackGroundCheckBox);
            this.SpectrometerParametersGroupBox.Controls.Add(this.scansLabel);
            this.SpectrometerParametersGroupBox.Controls.Add(this.informationLabel);
            this.SpectrometerParametersGroupBox.Controls.Add(this.adquisitionTimeTextBox);
            this.SpectrometerParametersGroupBox.Controls.Add(this.adquisitonLabel);
            this.SpectrometerParametersGroupBox.Controls.Add(this.adquisitionProgressLabel);
            this.SpectrometerParametersGroupBox.Controls.Add(this.adquisitionProgressBar);
            this.SpectrometerParametersGroupBox.Controls.Add(this.fftFIDCheckBox);
            this.SpectrometerParametersGroupBox.Controls.Add(this.button1);
            this.SpectrometerParametersGroupBox.Controls.Add(this.pulsePowerTextBox);
            this.SpectrometerParametersGroupBox.Controls.Add(this.recoveryTimeTextBox);
            this.SpectrometerParametersGroupBox.Controls.Add(this.triggerSourceInfo);
            this.SpectrometerParametersGroupBox.Controls.Add(this.triggerSourceInfoAsterisk);
            this.SpectrometerParametersGroupBox.Controls.Add(this.triggerSourceTextBox);
            this.SpectrometerParametersGroupBox.Controls.Add(this.slopeGroupBox);
            this.SpectrometerParametersGroupBox.Controls.Add(this.triggerSourceLabel);
            this.SpectrometerParametersGroupBox.Controls.Add(this.triggerLevelLabel);
            this.SpectrometerParametersGroupBox.Controls.Add(this.hysteresisLabel);
            this.SpectrometerParametersGroupBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.SpectrometerParametersGroupBox.Location = new System.Drawing.Point(12, 232);
            this.SpectrometerParametersGroupBox.Name = "SpectrometerParametersGroupBox";
            this.SpectrometerParametersGroupBox.Size = new System.Drawing.Size(232, 349);
            this.SpectrometerParametersGroupBox.TabIndex = 4;
            this.SpectrometerParametersGroupBox.TabStop = false;
            this.SpectrometerParametersGroupBox.Text = "Spectrometer Parameters";
            // 
            // responseLabel
            // 
            this.responseLabel.AutoSize = true;
            this.responseLabel.Location = new System.Drawing.Point(106, 333);
            this.responseLabel.Name = "responseLabel";
            this.responseLabel.Size = new System.Drawing.Size(93, 13);
            this.responseLabel.TabIndex = 20;
            this.responseLabel.Text = "NOTHING_READ";
            // 
            // setBackGroundCheckBox
            // 
            this.setBackGroundCheckBox.Appearance = System.Windows.Forms.Appearance.Button;
            this.setBackGroundCheckBox.AutoSize = true;
            this.setBackGroundCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.setBackGroundCheckBox.Location = new System.Drawing.Point(97, 214);
            this.setBackGroundCheckBox.Name = "setBackGroundCheckBox";
            this.setBackGroundCheckBox.Size = new System.Drawing.Size(94, 23);
            this.setBackGroundCheckBox.TabIndex = 19;
            this.setBackGroundCheckBox.Text = "Set Background";
            this.setBackGroundCheckBox.UseVisualStyleBackColor = true;
            this.setBackGroundCheckBox.CheckedChanged += new System.EventHandler(this.setBackGroundCheckBox_CheckedChanged);
            // 
            // scansLabel
            // 
            this.scansLabel.AutoSize = true;
            this.scansLabel.Location = new System.Drawing.Point(16, 333);
            this.scansLabel.Name = "scansLabel";
            this.scansLabel.Size = new System.Drawing.Size(76, 13);
            this.scansLabel.TabIndex = 18;
            this.scansLabel.Text = "Scans done: 0";
            // 
            // informationLabel
            // 
            this.informationLabel.AutoSize = true;
            this.informationLabel.Location = new System.Drawing.Point(16, 320);
            this.informationLabel.Name = "informationLabel";
            this.informationLabel.Size = new System.Drawing.Size(71, 13);
            this.informationLabel.TabIndex = 17;
            this.informationLabel.Text = "Nothing done";
            // 
            // adquisitionTimeTextBox
            // 
            this.adquisitionTimeTextBox.Enabled = false;
            this.adquisitionTimeTextBox.Location = new System.Drawing.Point(120, 102);
            this.adquisitionTimeTextBox.Name = "adquisitionTimeTextBox";
            this.adquisitionTimeTextBox.Size = new System.Drawing.Size(96, 20);
            this.adquisitionTimeTextBox.TabIndex = 16;
            this.adquisitionTimeTextBox.Text = "6553";
            // 
            // adquisitonLabel
            // 
            this.adquisitonLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.adquisitonLabel.Location = new System.Drawing.Point(16, 106);
            this.adquisitonLabel.Name = "adquisitonLabel";
            this.adquisitonLabel.Size = new System.Drawing.Size(112, 16);
            this.adquisitonLabel.TabIndex = 15;
            this.adquisitonLabel.Text = "Adquisition Time*:";
            // 
            // adquisitionProgressLabel
            // 
            this.adquisitionProgressLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.adquisitionProgressLabel.Location = new System.Drawing.Point(16, 273);
            this.adquisitionProgressLabel.Name = "adquisitionProgressLabel";
            this.adquisitionProgressLabel.Size = new System.Drawing.Size(131, 18);
            this.adquisitionProgressLabel.TabIndex = 14;
            this.adquisitionProgressLabel.Text = "Adquisition progress:";
            // 
            // adquisitionProgressBar
            // 
            this.adquisitionProgressBar.Location = new System.Drawing.Point(16, 294);
            this.adquisitionProgressBar.Name = "adquisitionProgressBar";
            this.adquisitionProgressBar.Size = new System.Drawing.Size(200, 23);
            this.adquisitionProgressBar.TabIndex = 13;
            // 
            // fftFIDCheckBox
            // 
            this.fftFIDCheckBox.Appearance = System.Windows.Forms.Appearance.Button;
            this.fftFIDCheckBox.AutoSize = true;
            this.fftFIDCheckBox.Checked = true;
            this.fftFIDCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.fftFIDCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.fftFIDCheckBox.Location = new System.Drawing.Point(97, 243);
            this.fftFIDCheckBox.Name = "fftFIDCheckBox";
            this.fftFIDCheckBox.Size = new System.Drawing.Size(58, 23);
            this.fftFIDCheckBox.TabIndex = 12;
            this.fftFIDCheckBox.Text = "FFT/FID";
            this.fftFIDCheckBox.UseVisualStyleBackColor = true;
            this.fftFIDCheckBox.CheckedChanged += new System.EventHandler(this.fftFIDCheckBox_CheckedChanged);
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.SystemColors.Control;
            this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button1.Location = new System.Drawing.Point(16, 214);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 56);
            this.button1.TabIndex = 11;
            this.button1.Text = "Start Adquisition";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // pulsePowerTextBox
            // 
            this.pulsePowerTextBox.Location = new System.Drawing.Point(120, 76);
            this.pulsePowerTextBox.Name = "pulsePowerTextBox";
            this.pulsePowerTextBox.Size = new System.Drawing.Size(96, 20);
            this.pulsePowerTextBox.TabIndex = 10;
            this.pulsePowerTextBox.Text = "10000";
            // 
            // recoveryTimeTextBox
            // 
            this.recoveryTimeTextBox.Location = new System.Drawing.Point(120, 50);
            this.recoveryTimeTextBox.Name = "recoveryTimeTextBox";
            this.recoveryTimeTextBox.Size = new System.Drawing.Size(96, 20);
            this.recoveryTimeTextBox.TabIndex = 9;
            this.recoveryTimeTextBox.Text = "10000";
            // 
            // triggerSourceInfo
            // 
            this.triggerSourceInfo.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.triggerSourceInfo.Location = new System.Drawing.Point(24, 179);
            this.triggerSourceInfo.Name = "triggerSourceInfo";
            this.triggerSourceInfo.Size = new System.Drawing.Size(192, 32);
            this.triggerSourceInfo.TabIndex = 8;
            this.triggerSourceInfo.Text = "These parameters are expressed in milisecond (ms).";
            // 
            // triggerSourceInfoAsterisk
            // 
            this.triggerSourceInfoAsterisk.Location = new System.Drawing.Point(13, 179);
            this.triggerSourceInfoAsterisk.Name = "triggerSourceInfoAsterisk";
            this.triggerSourceInfoAsterisk.Size = new System.Drawing.Size(16, 23);
            this.triggerSourceInfoAsterisk.TabIndex = 7;
            this.triggerSourceInfoAsterisk.Text = "*";
            // 
            // triggerSourceTextBox
            // 
            this.triggerSourceTextBox.Location = new System.Drawing.Point(120, 24);
            this.triggerSourceTextBox.Name = "triggerSourceTextBox";
            this.triggerSourceTextBox.Size = new System.Drawing.Size(96, 20);
            this.triggerSourceTextBox.TabIndex = 1;
            this.triggerSourceTextBox.Text = "650";
            this.triggerSourceTextBox.TextChanged += new System.EventHandler(this.triggerSourceTextBox_TextChanged);
            // 
            // slopeGroupBox
            // 
            this.slopeGroupBox.Controls.Add(this.slopeRisingButton);
            this.slopeGroupBox.Controls.Add(this.slopeFallingButton);
            this.slopeGroupBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.slopeGroupBox.Location = new System.Drawing.Point(16, 128);
            this.slopeGroupBox.Name = "slopeGroupBox";
            this.slopeGroupBox.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.slopeGroupBox.Size = new System.Drawing.Size(200, 48);
            this.slopeGroupBox.TabIndex = 6;
            this.slopeGroupBox.TabStop = false;
            this.slopeGroupBox.Text = "Slope";
            // 
            // slopeRisingButton
            // 
            this.slopeRisingButton.Checked = true;
            this.slopeRisingButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.slopeRisingButton.Location = new System.Drawing.Point(8, 16);
            this.slopeRisingButton.Name = "slopeRisingButton";
            this.slopeRisingButton.Size = new System.Drawing.Size(56, 24);
            this.slopeRisingButton.TabIndex = 0;
            this.slopeRisingButton.TabStop = true;
            this.slopeRisingButton.Text = "Rising";
            // 
            // slopeFallingButton
            // 
            this.slopeFallingButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.slopeFallingButton.Location = new System.Drawing.Point(72, 16);
            this.slopeFallingButton.Name = "slopeFallingButton";
            this.slopeFallingButton.Size = new System.Drawing.Size(56, 24);
            this.slopeFallingButton.TabIndex = 1;
            this.slopeFallingButton.Text = "Falling";
            // 
            // triggerSourceLabel
            // 
            this.triggerSourceLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.triggerSourceLabel.Location = new System.Drawing.Point(16, 28);
            this.triggerSourceLabel.Name = "triggerSourceLabel";
            this.triggerSourceLabel.Size = new System.Drawing.Size(88, 16);
            this.triggerSourceLabel.TabIndex = 0;
            this.triggerSourceLabel.Text = "Scans:";
            // 
            // triggerLevelLabel
            // 
            this.triggerLevelLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.triggerLevelLabel.Location = new System.Drawing.Point(16, 54);
            this.triggerLevelLabel.Name = "triggerLevelLabel";
            this.triggerLevelLabel.Size = new System.Drawing.Size(112, 16);
            this.triggerLevelLabel.TabIndex = 2;
            this.triggerLevelLabel.Text = "Recovery Time*:";
            // 
            // hysteresisLabel
            // 
            this.hysteresisLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.hysteresisLabel.Location = new System.Drawing.Point(16, 80);
            this.hysteresisLabel.Name = "hysteresisLabel";
            this.hysteresisLabel.Size = new System.Drawing.Size(112, 16);
            this.hysteresisLabel.TabIndex = 4;
            this.hysteresisLabel.Text = "Pulse Power*:";
            // 
            // channelParametersGroupBox
            // 
            this.channelParametersGroupBox.Controls.Add(this.physicalChannelComboBox);
            this.channelParametersGroupBox.Controls.Add(this.minimumValueNumeric);
            this.channelParametersGroupBox.Controls.Add(this.maximumValueNumeric);
            this.channelParametersGroupBox.Controls.Add(this.maximumLabel);
            this.channelParametersGroupBox.Controls.Add(this.minimumLabel);
            this.channelParametersGroupBox.Controls.Add(this.physicalChannelLabel);
            this.channelParametersGroupBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.channelParametersGroupBox.Location = new System.Drawing.Point(12, 12);
            this.channelParametersGroupBox.Name = "channelParametersGroupBox";
            this.channelParametersGroupBox.Size = new System.Drawing.Size(232, 120);
            this.channelParametersGroupBox.TabIndex = 3;
            this.channelParametersGroupBox.TabStop = false;
            this.channelParametersGroupBox.Text = "Channel Parameters";
            // 
            // physicalChannelComboBox
            // 
            this.physicalChannelComboBox.Location = new System.Drawing.Point(120, 24);
            this.physicalChannelComboBox.Name = "physicalChannelComboBox";
            this.physicalChannelComboBox.Size = new System.Drawing.Size(96, 21);
            this.physicalChannelComboBox.TabIndex = 1;
            this.physicalChannelComboBox.Text = "Dev1/ai0";
            this.physicalChannelComboBox.SelectedIndexChanged += new System.EventHandler(this.physicalChannelComboBox_SelectedIndexChanged);
            this.physicalChannelComboBox.SelectedValueChanged += new System.EventHandler(this.physicalChannelComboBox_SelectedValueChanged);
            // 
            // minimumValueNumeric
            // 
            this.minimumValueNumeric.DecimalPlaces = 2;
            this.minimumValueNumeric.Location = new System.Drawing.Point(120, 56);
            this.minimumValueNumeric.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.minimumValueNumeric.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            -2147483648});
            this.minimumValueNumeric.Name = "minimumValueNumeric";
            this.minimumValueNumeric.Size = new System.Drawing.Size(96, 20);
            this.minimumValueNumeric.TabIndex = 3;
            this.minimumValueNumeric.Value = new decimal(new int[] {
            100,
            0,
            0,
            -2147418112});
            // 
            // maximumValueNumeric
            // 
            this.maximumValueNumeric.DecimalPlaces = 2;
            this.maximumValueNumeric.Location = new System.Drawing.Point(120, 88);
            this.maximumValueNumeric.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.maximumValueNumeric.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            -2147483648});
            this.maximumValueNumeric.Name = "maximumValueNumeric";
            this.maximumValueNumeric.Size = new System.Drawing.Size(96, 20);
            this.maximumValueNumeric.TabIndex = 5;
            this.maximumValueNumeric.Value = new decimal(new int[] {
            100,
            0,
            0,
            65536});
            // 
            // maximumLabel
            // 
            this.maximumLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.maximumLabel.Location = new System.Drawing.Point(16, 90);
            this.maximumLabel.Name = "maximumLabel";
            this.maximumLabel.Size = new System.Drawing.Size(96, 16);
            this.maximumLabel.TabIndex = 4;
            this.maximumLabel.Text = "Maximum (Volts):";
            // 
            // minimumLabel
            // 
            this.minimumLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.minimumLabel.Location = new System.Drawing.Point(16, 58);
            this.minimumLabel.Name = "minimumLabel";
            this.minimumLabel.Size = new System.Drawing.Size(96, 16);
            this.minimumLabel.TabIndex = 2;
            this.minimumLabel.Text = "Minimum (Volts):";
            // 
            // physicalChannelLabel
            // 
            this.physicalChannelLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.physicalChannelLabel.Location = new System.Drawing.Point(16, 26);
            this.physicalChannelLabel.Name = "physicalChannelLabel";
            this.physicalChannelLabel.Size = new System.Drawing.Size(96, 16);
            this.physicalChannelLabel.TabIndex = 0;
            this.physicalChannelLabel.Text = "Physical Channel:";
            // 
            // timingParametersGroupBox
            // 
            this.timingParametersGroupBox.Controls.Add(this.rateLabel);
            this.timingParametersGroupBox.Controls.Add(this.samplesLabel);
            this.timingParametersGroupBox.Controls.Add(this.samplesPerChannelNumeric);
            this.timingParametersGroupBox.Controls.Add(this.rateNumeric);
            this.timingParametersGroupBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.timingParametersGroupBox.Location = new System.Drawing.Point(12, 138);
            this.timingParametersGroupBox.Name = "timingParametersGroupBox";
            this.timingParametersGroupBox.Size = new System.Drawing.Size(232, 88);
            this.timingParametersGroupBox.TabIndex = 2;
            this.timingParametersGroupBox.TabStop = false;
            this.timingParametersGroupBox.Text = "Timing Parameters";
            // 
            // rateLabel
            // 
            this.rateLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.rateLabel.Location = new System.Drawing.Point(16, 56);
            this.rateLabel.Name = "rateLabel";
            this.rateLabel.Size = new System.Drawing.Size(64, 16);
            this.rateLabel.TabIndex = 2;
            this.rateLabel.Text = "Rate (Hz):";
            // 
            // samplesLabel
            // 
            this.samplesLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.samplesLabel.Location = new System.Drawing.Point(16, 24);
            this.samplesLabel.Name = "samplesLabel";
            this.samplesLabel.Size = new System.Drawing.Size(104, 16);
            this.samplesLabel.TabIndex = 0;
            this.samplesLabel.Text = "Samples / Channel:";
            // 
            // samplesPerChannelNumeric
            // 
            this.samplesPerChannelNumeric.Location = new System.Drawing.Point(120, 24);
            this.samplesPerChannelNumeric.Maximum = new decimal(new int[] {
            131072,
            0,
            0,
            0});
            this.samplesPerChannelNumeric.Name = "samplesPerChannelNumeric";
            this.samplesPerChannelNumeric.Size = new System.Drawing.Size(96, 20);
            this.samplesPerChannelNumeric.TabIndex = 1;
            this.samplesPerChannelNumeric.Value = new decimal(new int[] {
            131072,
            0,
            0,
            0});
            // 
            // rateNumeric
            // 
            this.rateNumeric.DecimalPlaces = 2;
            this.rateNumeric.Location = new System.Drawing.Point(120, 56);
            this.rateNumeric.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.rateNumeric.Name = "rateNumeric";
            this.rateNumeric.Size = new System.Drawing.Size(96, 20);
            this.rateNumeric.TabIndex = 3;
            this.rateNumeric.Value = new decimal(new int[] {
            20000,
            0,
            0,
            0});
            // 
            // mainPlotScottPlot
            // 
            this.mainPlotScottPlot.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainPlotScottPlot.Location = new System.Drawing.Point(0, 0);
            this.mainPlotScottPlot.Name = "mainPlotScottPlot";
            this.mainPlotScottPlot.Size = new System.Drawing.Size(696, 593);
            this.mainPlotScottPlot.TabIndex = 2;
            // 
            // adquisitionTimer
            // 
            this.adquisitionTimer.Tick += new System.EventHandler(this.adquisitionTimer_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(959, 593);
            this.Controls.Add(this.splitContainer1);
            this.Name = "Form1";
            this.Text = "Earth Fiel NMR spectrometer";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.SpectrometerParametersGroupBox.ResumeLayout(false);
            this.SpectrometerParametersGroupBox.PerformLayout();
            this.slopeGroupBox.ResumeLayout(false);
            this.channelParametersGroupBox.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.minimumValueNumeric)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.maximumValueNumeric)).EndInit();
            this.timingParametersGroupBox.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.samplesPerChannelNumeric)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.rateNumeric)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.GroupBox SpectrometerParametersGroupBox;
        private System.Windows.Forms.Label triggerSourceInfo;
        private System.Windows.Forms.Label triggerSourceInfoAsterisk;
        private System.Windows.Forms.TextBox triggerSourceTextBox;
        private System.Windows.Forms.GroupBox slopeGroupBox;
        private System.Windows.Forms.RadioButton slopeRisingButton;
        private System.Windows.Forms.RadioButton slopeFallingButton;
        private System.Windows.Forms.Label triggerSourceLabel;
        private System.Windows.Forms.Label triggerLevelLabel;
        private System.Windows.Forms.Label hysteresisLabel;
        internal System.Windows.Forms.GroupBox channelParametersGroupBox;
        private System.Windows.Forms.ComboBox physicalChannelComboBox;
        internal System.Windows.Forms.NumericUpDown minimumValueNumeric;
        internal System.Windows.Forms.NumericUpDown maximumValueNumeric;
        internal System.Windows.Forms.Label maximumLabel;
        internal System.Windows.Forms.Label minimumLabel;
        internal System.Windows.Forms.Label physicalChannelLabel;
        private System.Windows.Forms.GroupBox timingParametersGroupBox;
        private System.Windows.Forms.Label rateLabel;
        private System.Windows.Forms.Label samplesLabel;
        private System.Windows.Forms.NumericUpDown samplesPerChannelNumeric;
        private System.Windows.Forms.NumericUpDown rateNumeric;
        private System.Windows.Forms.TextBox pulsePowerTextBox;
        private System.Windows.Forms.TextBox recoveryTimeTextBox;
        private System.Windows.Forms.CheckBox fftFIDCheckBox;
        private System.Windows.Forms.Button button1;
        private ScottPlot.FormsPlot mainPlotScottPlot;
        private System.Windows.Forms.Label adquisitionProgressLabel;
        private System.Windows.Forms.ProgressBar adquisitionProgressBar;
        private System.Windows.Forms.TextBox adquisitionTimeTextBox;
        private System.Windows.Forms.Label adquisitonLabel;
        private System.Windows.Forms.Timer adquisitionTimer;
        private System.Windows.Forms.Label informationLabel;
        private System.Windows.Forms.Label scansLabel;
        private System.Windows.Forms.CheckBox setBackGroundCheckBox;
        private System.Windows.Forms.Label responseLabel;
    }
}

