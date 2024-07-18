namespace EFNMR {
  partial class Form1 {
    private System.ComponentModel.IContainer components = null;
    protected override void Dispose(bool disposing) {
      if (disposing && (components != null)) {
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
      this.mainPlotScottPlot = new ScottPlot.FormsPlot();
      this.SpectrometerParametersGroupBox = new System.Windows.Forms.GroupBox();
      this.setBackgroundBtn = new System.Windows.Forms.Button();
      this.label5 = new System.Windows.Forms.Label();
      this.label6 = new System.Windows.Forms.Label();
      this.label7 = new System.Windows.Forms.Label();
      this.label4 = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.label1 = new System.Windows.Forms.Label();
      this.StartCaptureBtn = new System.Windows.Forms.Button();
      this.samplerateLabel = new System.Windows.Forms.Label();
      this.pulseTimeNumeric = new System.Windows.Forms.NumericUpDown();
      this.adquisitionTimeText = new System.Windows.Forms.Label();
      this.recoveryTimeNumeric = new System.Windows.Forms.NumericUpDown();
      this.ScanCountNumeric = new System.Windows.Forms.NumericUpDown();
      this.samplesNumeric = new System.Windows.Forms.NumericUpDown();
      this.samplesLabel = new System.Windows.Forms.Label();
      this.informationLabel = new System.Windows.Forms.Label();
      this.adquisitonLabel = new System.Windows.Forms.Label();
      this.fftFIDCheckBox = new System.Windows.Forms.CheckBox();
      this.triggerSourceInfo = new System.Windows.Forms.Label();
      this.triggerSourceInfoAsterisk = new System.Windows.Forms.Label();
      this.triggerSourceLabel = new System.Windows.Forms.Label();
      this.triggerLevelLabel = new System.Windows.Forms.Label();
      this.hysteresisLabel = new System.Windows.Forms.Label();
      this.splitContainer1 = new System.Windows.Forms.SplitContainer();
      this.SpectrometerParametersGroupBox.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.pulseTimeNumeric)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.recoveryTimeNumeric)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.ScanCountNumeric)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.samplesNumeric)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
      this.splitContainer1.Panel1.SuspendLayout();
      this.splitContainer1.Panel2.SuspendLayout();
      this.splitContainer1.SuspendLayout();
      this.SuspendLayout();
      // 
      // mainPlotScottPlot
      // 
      this.mainPlotScottPlot.Dock = System.Windows.Forms.DockStyle.Fill;
      this.mainPlotScottPlot.Location = new System.Drawing.Point(0, 0);
      this.mainPlotScottPlot.Name = "mainPlotScottPlot";
      this.mainPlotScottPlot.Size = new System.Drawing.Size(738, 593);
      this.mainPlotScottPlot.TabIndex = 2;
      // 
      // SpectrometerParametersGroupBox
      // 
      this.SpectrometerParametersGroupBox.Controls.Add(this.setBackgroundBtn);
      this.SpectrometerParametersGroupBox.Controls.Add(this.label5);
      this.SpectrometerParametersGroupBox.Controls.Add(this.label6);
      this.SpectrometerParametersGroupBox.Controls.Add(this.label7);
      this.SpectrometerParametersGroupBox.Controls.Add(this.label4);
      this.SpectrometerParametersGroupBox.Controls.Add(this.label3);
      this.SpectrometerParametersGroupBox.Controls.Add(this.label2);
      this.SpectrometerParametersGroupBox.Controls.Add(this.label1);
      this.SpectrometerParametersGroupBox.Controls.Add(this.StartCaptureBtn);
      this.SpectrometerParametersGroupBox.Controls.Add(this.samplerateLabel);
      this.SpectrometerParametersGroupBox.Controls.Add(this.pulseTimeNumeric);
      this.SpectrometerParametersGroupBox.Controls.Add(this.adquisitionTimeText);
      this.SpectrometerParametersGroupBox.Controls.Add(this.recoveryTimeNumeric);
      this.SpectrometerParametersGroupBox.Controls.Add(this.ScanCountNumeric);
      this.SpectrometerParametersGroupBox.Controls.Add(this.samplesNumeric);
      this.SpectrometerParametersGroupBox.Controls.Add(this.samplesLabel);
      this.SpectrometerParametersGroupBox.Controls.Add(this.informationLabel);
      this.SpectrometerParametersGroupBox.Controls.Add(this.adquisitonLabel);
      this.SpectrometerParametersGroupBox.Controls.Add(this.fftFIDCheckBox);
      this.SpectrometerParametersGroupBox.Controls.Add(this.triggerSourceInfo);
      this.SpectrometerParametersGroupBox.Controls.Add(this.triggerSourceInfoAsterisk);
      this.SpectrometerParametersGroupBox.Controls.Add(this.triggerSourceLabel);
      this.SpectrometerParametersGroupBox.Controls.Add(this.triggerLevelLabel);
      this.SpectrometerParametersGroupBox.Controls.Add(this.hysteresisLabel);
      this.SpectrometerParametersGroupBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
      this.SpectrometerParametersGroupBox.Location = new System.Drawing.Point(12, 12);
      this.SpectrometerParametersGroupBox.Name = "SpectrometerParametersGroupBox";
      this.SpectrometerParametersGroupBox.Size = new System.Drawing.Size(198, 352);
      this.SpectrometerParametersGroupBox.TabIndex = 4;
      this.SpectrometerParametersGroupBox.TabStop = false;
      this.SpectrometerParametersGroupBox.Text = "Spectrometer Parameters";
      // 
      // setBackgroundBtn
      // 
      this.setBackgroundBtn.BackColor = System.Drawing.SystemColors.Control;
      this.setBackgroundBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
      this.setBackgroundBtn.Location = new System.Drawing.Point(6, 214);
      this.setBackgroundBtn.Name = "setBackgroundBtn";
      this.setBackgroundBtn.Size = new System.Drawing.Size(112, 22);
      this.setBackgroundBtn.TabIndex = 31;
      this.setBackgroundBtn.Text = "Set Background";
      this.setBackgroundBtn.UseVisualStyleBackColor = false;
      this.setBackgroundBtn.Click += new System.EventHandler(this.setBackgroundBtn_Click);
      // 
      // label5
      // 
      this.label5.Location = new System.Drawing.Point(98, 315);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(94, 17);
      this.label5.TabIndex = 30;
      this.label5.Text = "0.02 uT";
      this.label5.TextAlign = System.Drawing.ContentAlignment.TopRight;
      // 
      // label6
      // 
      this.label6.Location = new System.Drawing.Point(98, 298);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(94, 17);
      this.label6.TabIndex = 29;
      this.label6.Text = "40.21 uT";
      this.label6.TextAlign = System.Drawing.ContentAlignment.TopRight;
      // 
      // label7
      // 
      this.label7.Location = new System.Drawing.Point(98, 332);
      this.label7.Name = "label7";
      this.label7.Size = new System.Drawing.Size(94, 17);
      this.label7.TabIndex = 28;
      this.label7.Text = "-0.07 uT";
      this.label7.TextAlign = System.Drawing.ContentAlignment.TopRight;
      // 
      // label4
      // 
      this.label4.Location = new System.Drawing.Point(6, 315);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(94, 17);
      this.label4.TabIndex = 27;
      this.label4.Text = "Y:";
      // 
      // label3
      // 
      this.label3.Location = new System.Drawing.Point(6, 298);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(94, 17);
      this.label3.TabIndex = 26;
      this.label3.Text = "X:";
      // 
      // label2
      // 
      this.label2.Location = new System.Drawing.Point(6, 332);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(94, 17);
      this.label2.TabIndex = 25;
      this.label2.Text = "Z:";
      // 
      // label1
      // 
      this.label1.Location = new System.Drawing.Point(112, 42);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(80, 16);
      this.label1.TabIndex = 24;
      this.label1.Text = " 50000 SPS";
      this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
      // 
      // StartCaptureBtn
      // 
      this.StartCaptureBtn.BackColor = System.Drawing.SystemColors.Control;
      this.StartCaptureBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
      this.StartCaptureBtn.Location = new System.Drawing.Point(6, 185);
      this.StartCaptureBtn.Name = "StartCaptureBtn";
      this.StartCaptureBtn.Size = new System.Drawing.Size(186, 22);
      this.StartCaptureBtn.TabIndex = 11;
      this.StartCaptureBtn.Text = "Start Adquisition";
      this.StartCaptureBtn.UseVisualStyleBackColor = false;
      this.StartCaptureBtn.Click += new System.EventHandler(this.StartCapture_Click);
      // 
      // samplerateLabel
      // 
      this.samplerateLabel.Location = new System.Drawing.Point(6, 44);
      this.samplerateLabel.Name = "samplerateLabel";
      this.samplerateLabel.Size = new System.Drawing.Size(112, 16);
      this.samplerateLabel.TabIndex = 23;
      this.samplerateLabel.Text = "Samplerate:";
      // 
      // pulseTimeNumeric
      // 
      this.pulseTimeNumeric.Location = new System.Drawing.Point(130, 129);
      this.pulseTimeNumeric.Maximum = new decimal(new int[] {
            32000,
            0,
            0,
            0});
      this.pulseTimeNumeric.Minimum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
      this.pulseTimeNumeric.Name = "pulseTimeNumeric";
      this.pulseTimeNumeric.Size = new System.Drawing.Size(62, 20);
      this.pulseTimeNumeric.TabIndex = 6;
      this.pulseTimeNumeric.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.pulseTimeNumeric.Value = new decimal(new int[] {
            10000,
            0,
            0,
            0});
      // 
      // adquisitionTimeText
      // 
      this.adquisitionTimeText.Location = new System.Drawing.Point(112, 58);
      this.adquisitionTimeText.Name = "adquisitionTimeText";
      this.adquisitionTimeText.Size = new System.Drawing.Size(80, 16);
      this.adquisitionTimeText.TabIndex = 22;
      this.adquisitionTimeText.Text = "6553.00";
      this.adquisitionTimeText.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
      // 
      // recoveryTimeNumeric
      // 
      this.recoveryTimeNumeric.Location = new System.Drawing.Point(130, 103);
      this.recoveryTimeNumeric.Maximum = new decimal(new int[] {
            32000,
            0,
            0,
            0});
      this.recoveryTimeNumeric.Minimum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
      this.recoveryTimeNumeric.Name = "recoveryTimeNumeric";
      this.recoveryTimeNumeric.Size = new System.Drawing.Size(62, 20);
      this.recoveryTimeNumeric.TabIndex = 5;
      this.recoveryTimeNumeric.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.recoveryTimeNumeric.Value = new decimal(new int[] {
            10000,
            0,
            0,
            0});
      // 
      // ScanCountNumeric
      // 
      this.ScanCountNumeric.Location = new System.Drawing.Point(130, 77);
      this.ScanCountNumeric.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
      this.ScanCountNumeric.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
      this.ScanCountNumeric.Name = "ScanCountNumeric";
      this.ScanCountNumeric.Size = new System.Drawing.Size(62, 20);
      this.ScanCountNumeric.TabIndex = 21;
      this.ScanCountNumeric.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.ScanCountNumeric.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
      // 
      // samplesNumeric
      // 
      this.samplesNumeric.Increment = new decimal(new int[] {
            32,
            0,
            0,
            0});
      this.samplesNumeric.Location = new System.Drawing.Point(130, 19);
      this.samplesNumeric.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
      this.samplesNumeric.Name = "samplesNumeric";
      this.samplesNumeric.Size = new System.Drawing.Size(62, 20);
      this.samplesNumeric.TabIndex = 1;
      this.samplesNumeric.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
      this.samplesNumeric.Value = new decimal(new int[] {
            262144,
            0,
            0,
            0});
      this.samplesNumeric.ValueChanged += new System.EventHandler(this.samplesNumeric_ValueChanged);
      // 
      // samplesLabel
      // 
      this.samplesLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
      this.samplesLabel.Location = new System.Drawing.Point(6, 21);
      this.samplesLabel.Name = "samplesLabel";
      this.samplesLabel.Size = new System.Drawing.Size(104, 16);
      this.samplesLabel.TabIndex = 0;
      this.samplesLabel.Text = "Samples / Channel:";
      // 
      // informationLabel
      // 
      this.informationLabel.Location = new System.Drawing.Point(6, 239);
      this.informationLabel.Name = "informationLabel";
      this.informationLabel.Size = new System.Drawing.Size(186, 17);
      this.informationLabel.TabIndex = 17;
      this.informationLabel.Text = "Nothing done";
      // 
      // adquisitonLabel
      // 
      this.adquisitonLabel.Location = new System.Drawing.Point(6, 60);
      this.adquisitonLabel.Name = "adquisitonLabel";
      this.adquisitonLabel.Size = new System.Drawing.Size(112, 16);
      this.adquisitonLabel.TabIndex = 15;
      this.adquisitonLabel.Text = "Adquisition Time*:";
      // 
      // fftFIDCheckBox
      // 
      this.fftFIDCheckBox.AutoSize = true;
      this.fftFIDCheckBox.Location = new System.Drawing.Point(125, 217);
      this.fftFIDCheckBox.Name = "fftFIDCheckBox";
      this.fftFIDCheckBox.Size = new System.Drawing.Size(67, 17);
      this.fftFIDCheckBox.TabIndex = 12;
      this.fftFIDCheckBox.Text = "FFT/FID";
      this.fftFIDCheckBox.UseVisualStyleBackColor = true;
      this.fftFIDCheckBox.CheckedChanged += new System.EventHandler(this.fftFIDCheckBox_CheckedChanged);
      // 
      // triggerSourceInfo
      // 
      this.triggerSourceInfo.FlatStyle = System.Windows.Forms.FlatStyle.System;
      this.triggerSourceInfo.Location = new System.Drawing.Point(16, 152);
      this.triggerSourceInfo.Name = "triggerSourceInfo";
      this.triggerSourceInfo.Size = new System.Drawing.Size(176, 30);
      this.triggerSourceInfo.TabIndex = 8;
      this.triggerSourceInfo.Text = "These parameters are expressed in milisecond (ms).";
      // 
      // triggerSourceInfoAsterisk
      // 
      this.triggerSourceInfoAsterisk.Location = new System.Drawing.Point(6, 152);
      this.triggerSourceInfoAsterisk.Name = "triggerSourceInfoAsterisk";
      this.triggerSourceInfoAsterisk.Size = new System.Drawing.Size(16, 23);
      this.triggerSourceInfoAsterisk.TabIndex = 7;
      this.triggerSourceInfoAsterisk.Text = "*";
      // 
      // triggerSourceLabel
      // 
      this.triggerSourceLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
      this.triggerSourceLabel.Location = new System.Drawing.Point(6, 79);
      this.triggerSourceLabel.Name = "triggerSourceLabel";
      this.triggerSourceLabel.Size = new System.Drawing.Size(88, 16);
      this.triggerSourceLabel.TabIndex = 0;
      this.triggerSourceLabel.Text = "Scans:";
      // 
      // triggerLevelLabel
      // 
      this.triggerLevelLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
      this.triggerLevelLabel.Location = new System.Drawing.Point(6, 105);
      this.triggerLevelLabel.Name = "triggerLevelLabel";
      this.triggerLevelLabel.Size = new System.Drawing.Size(112, 16);
      this.triggerLevelLabel.TabIndex = 2;
      this.triggerLevelLabel.Text = "Recovery Time*:";
      // 
      // hysteresisLabel
      // 
      this.hysteresisLabel.FlatStyle = System.Windows.Forms.FlatStyle.System;
      this.hysteresisLabel.Location = new System.Drawing.Point(6, 131);
      this.hysteresisLabel.Name = "hysteresisLabel";
      this.hysteresisLabel.Size = new System.Drawing.Size(112, 16);
      this.hysteresisLabel.TabIndex = 4;
      this.hysteresisLabel.Text = "Pulse Time*:";
      // 
      // splitContainer1
      // 
      this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
      this.splitContainer1.IsSplitterFixed = true;
      this.splitContainer1.Location = new System.Drawing.Point(0, 0);
      this.splitContainer1.Name = "splitContainer1";
      // 
      // splitContainer1.Panel1
      // 
      this.splitContainer1.Panel1.Controls.Add(this.SpectrometerParametersGroupBox);
      // 
      // splitContainer1.Panel2
      // 
      this.splitContainer1.Panel2.Controls.Add(this.mainPlotScottPlot);
      this.splitContainer1.Size = new System.Drawing.Size(959, 593);
      this.splitContainer1.SplitterDistance = 217;
      this.splitContainer1.TabIndex = 1;
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
      this.SpectrometerParametersGroupBox.ResumeLayout(false);
      this.SpectrometerParametersGroupBox.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.pulseTimeNumeric)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.recoveryTimeNumeric)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.ScanCountNumeric)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.samplesNumeric)).EndInit();
      this.splitContainer1.Panel1.ResumeLayout(false);
      this.splitContainer1.Panel2.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
      this.splitContainer1.ResumeLayout(false);
      this.ResumeLayout(false);

        }

    #endregion

    private ScottPlot.FormsPlot mainPlotScottPlot;
    private System.Windows.Forms.GroupBox SpectrometerParametersGroupBox;
    private System.Windows.Forms.NumericUpDown samplesNumeric;
    private System.Windows.Forms.Label samplesLabel;
    private System.Windows.Forms.Label informationLabel;
    private System.Windows.Forms.Label adquisitonLabel;
    private System.Windows.Forms.CheckBox fftFIDCheckBox;
    private System.Windows.Forms.Button StartCaptureBtn;
    private System.Windows.Forms.Label triggerSourceInfo;
    private System.Windows.Forms.Label triggerSourceInfoAsterisk;
    private System.Windows.Forms.Label triggerSourceLabel;
    private System.Windows.Forms.Label triggerLevelLabel;
    private System.Windows.Forms.Label hysteresisLabel;
    private System.Windows.Forms.SplitContainer splitContainer1;
    private System.Windows.Forms.NumericUpDown ScanCountNumeric;
    private System.Windows.Forms.Label adquisitionTimeText;
    private System.Windows.Forms.NumericUpDown pulseTimeNumeric;
    private System.Windows.Forms.NumericUpDown recoveryTimeNumeric;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Label samplerateLabel;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.Label label6;
    private System.Windows.Forms.Label label7;
    private System.Windows.Forms.Button setBackgroundBtn;
  }
}

