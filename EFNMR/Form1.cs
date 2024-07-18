using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ScottPlot;
using System.Diagnostics;
using System.IO;
using FftSharp;
using MathNet.Filtering;
using MathNet.Numerics;
using MathNet.Filtering.FIR;
using System.Security.Cryptography;
using System.Runtime.InteropServices.ComTypes;
using System.IO.Ports;
using LibUsbDotNet;
using LibUsbDotNet.Main;
using Newtonsoft.Json;
using LibUsbDotNet.Info;
using LibUsbDotNet.LibUsb;
using System.Threading;
namespace EFNMR {
  public partial class Form1 : Form {
    private string        pFilename = "background.nmr";
    private const int     pVID = 0x0547;
    private const int     pPID = 0x1002;
    private const double  pLowFrequency = 1000.0;
    private const double  pHighFrequency = 3000.0;
    private const int     pSamplerate = 50000;
    private UsbDevice     pDeviceNMR = null;
    private Plot          pPlot = new ScottPlot.Plot();
    private double[]      pBackgroundNoise;
    private double[]      pSignal;
    public Form1() {
      InitializeComponent();
      UsbDevice.ForceLibUsbWinBack = true;
      UsbDeviceFinder USBFinder = new UsbDeviceFinder(pVID, pPID);
      pDeviceNMR = UsbDevice.OpenUsbDevice(USBFinder);
      if (pDeviceNMR == null) return;
      IUsbDevice WholeDevice = pDeviceNMR as IUsbDevice;
      if (!ReferenceEquals(WholeDevice, null)) {
        WholeDevice.SetConfiguration(1);
        WholeDevice.ClaimInterface(0);
      }
    }
    private static void WriteToFile(string filename, double[] array) {
      using (StreamWriter file = new StreamWriter(filename)) {
        string json = JsonConvert.SerializeObject(array);
        file.Write(json);
      }
    }
    private static double[] ReadFromFile(string filename) {
      using (StreamReader file = new StreamReader(filename)) {
        string json = file.ReadToEnd();
        return JsonConvert.DeserializeObject<double[]>(json);
      }
    }
    private void Form1_Load(object sender, EventArgs e) {           
      pPlot = mainPlotScottPlot.Plot;
      pPlot.Style(Style.Seaborn);
      pPlot.Title("Earth Field NMR spectrometer");
      pPlot.XLabel("Time/s");
      pPlot.YLabel("Intensity/a.u.");
      pPlot.AxisAuto();
      mainPlotScottPlot.Refresh();
      StartCaptureBtn.Enabled = (pDeviceNMR != null);
      adquisitionTimeText.Text = $"{((samplesNumeric.Value / pSamplerate) * 1000):F2} ms";
      if (pDeviceNMR == null) {
        informationLabel.Text = "No NMR device detected";
        informationLabel.ForeColor = System.Drawing.Color.Red;
        return;
      } else {
        informationLabel.Text = "NMR device detected";
        informationLabel.ForeColor = System.Drawing.Color.Green;
      }
      if (File.Exists(pFilename)) {
        pBackgroundNoise = ReadFromFile(pFilename);
      } else {
        informationLabel.Text = "Please, set background NMR signal";
        informationLabel.ForeColor = System.Drawing.Color.Red;
        fftFIDCheckBox.Enabled = false;
      }
    }
    private void samplesNumeric_ValueChanged(object sender, EventArgs e) {
      samplesNumeric.Value = (uint)samplesNumeric.Value & 0xFFFFFFE0;
      adquisitionTimeText.Text = $"{((samplesNumeric.Value / pSamplerate) * 1000):F2} ms";
    }
    private async void StartCapture_Click(object sender, EventArgs e) {
      SpectrometerParametersGroupBox.Enabled = false;
      ushort ScanCount = Convert.ToUInt16(ScanCountNumeric.Value);
      uint SamplesCount = Convert.ToUInt32(samplesNumeric.Value);
      double[] Signal = new double[SamplesCount];
      pSignal = new double[SamplesCount];
      for (int n = 0; n < SamplesCount; n++) Signal[n] = 0;
      await System.Threading.Tasks.Task.Run(() => {
        ErrorCode Error = ErrorCode.None;
        UsbEndpointReader Reader = pDeviceNMR.OpenEndpointReader(ReadEndpointID.Ep01);
        int TransferredLength = 0;
        UsbSetupPacket SetupPacket = new UsbSetupPacket {
          RequestType = (byte)(UsbCtrlFlags.Direction_Out | UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device),
          Request = 0x20,
          Value = (short)recoveryTimeNumeric.Value,
          Index = (short)pulseTimeNumeric.Value,
          Length = 0
        };
        if (pDeviceNMR.ControlTransfer(ref SetupPacket, null, 0, out TransferredLength) == true) {
          Console.WriteLine($"Control Transfer Successful");
        } else {
          Invoke(new MethodInvoker(() => {
            informationLabel.Text = "Control transfer failed";
            informationLabel.ForeColor = System.Drawing.Color.Red;
            SpectrometerParametersGroupBox.Enabled =  true;
          }));
          return;
        }
        SetupPacket = new UsbSetupPacket {
          RequestType = (byte)(UsbCtrlFlags.Direction_Out | UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device),
          Request = 0x21,
          Value = (short)20,
          Index = (short)10,
          Length = 0
        };
        if (pDeviceNMR.ControlTransfer(ref SetupPacket, null, 0, out TransferredLength) == true) {
          Console.WriteLine($"Control Transfer Successful");
        } else {
          Invoke(new MethodInvoker(() => {
            informationLabel.Text = "Control transfer failed";
            informationLabel.ForeColor = System.Drawing.Color.Red;
            SpectrometerParametersGroupBox.Enabled = true;
          }));
          return;
        }
        SetupPacket = new UsbSetupPacket {
          RequestType = (byte)(UsbCtrlFlags.Direction_Out | UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device),
          Request = 0x22,
          Value = (short)(SamplesCount & 0xFFFF),
          Index = (short)(SamplesCount >> 16),
          Length = 0
        };
        if (pDeviceNMR.ControlTransfer(ref SetupPacket, null, 0, out TransferredLength) == true) {
          Console.WriteLine($"Control Transfer Successful");
        } else {
          Invoke(new MethodInvoker(() => {
            informationLabel.Text = "Control transfer failed";
            informationLabel.ForeColor = System.Drawing.Color.Red;
            SpectrometerParametersGroupBox.Enabled = true;
          }));
          return;
        }
        SetupPacket = new UsbSetupPacket {
          RequestType = (byte)(UsbCtrlFlags.Direction_Out | UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device),
          Request     = 0x23,
          Value       = (short)ScanCount,
          Index       = (short)0,
          Length      = 0
        };
        Reader.ReadFlush();
        Reader.ReadThreadPriority = ThreadPriority.Highest;
        UsbTransferQueue TransferQeue = new UsbTransferQueue(Reader, 1024, 4096, 500000, 0);
        if (pDeviceNMR.ControlTransfer(ref SetupPacket, null, 0, out TransferredLength) == true) {
          Console.WriteLine($"Control Transfer Successful");
        } else {
          Invoke(new MethodInvoker(() => {
            informationLabel.Text = "Control transfer failed";
            informationLabel.ForeColor = System.Drawing.Color.Red;
            SpectrometerParametersGroupBox.Enabled = true;
          }));
          return;
        }
        int CurrentScan = 0;
        int CurrentSample = 0;
        int PrevReportedScan = 0;
        Invoke(new MethodInvoker(() => {
          informationLabel.Text = $"Scan {CurrentScan + 1} in progress...";
          informationLabel.ForeColor = System.Drawing.Color.Green;
        }));
        while (CurrentScan != ScanCount) {
          UsbTransferQueue.Handle Handle;
          Error = TransferQeue.Transfer(out Handle);
          if (Error != ErrorCode.Success) throw new Exception("Failed getting async result");
          for (int i = 0; i < Handle.Transferred; i += 64) {
            ushort[] BlockU = new ushort[32];
            short[]  BlockS = new short[32];
            Buffer.BlockCopy(Handle.Data, i, BlockU, 0, 64);
            Buffer.BlockCopy(Handle.Data, i, BlockS, 0, 64);
            ushort Count =  (ushort)(BlockU[31] & 0x001F);
            ushort ReadedScan = (ushort)(BlockU[31] >> 5);
            if (ReadedScan != PrevReportedScan) {
              Console.WriteLine($"Device start scan {ReadedScan}");
              PrevReportedScan = ReadedScan;
            }
            if (Count > 31) throw new Exception("Invalid block samples count");
            for (int j = 0; j < Count; j++) {
              if (CurrentSample < SamplesCount) {
                Signal[CurrentSample] += BlockS[j] / 32768.0;
                CurrentSample++;
                if(CurrentSample == SamplesCount) {
                  CurrentSample = 0;
                  CurrentScan++;
                  if (CurrentScan == ScanCount) {
                    Invoke(new MethodInvoker(() => {
                      for (int n = 0; n < SamplesCount; n++) pSignal[n] = Signal[n] / CurrentScan;
                      UpdatePlot();
                      informationLabel.Text = $"Done";
                      informationLabel.ForeColor = System.Drawing.Color.Green;
                      SpectrometerParametersGroupBox.Enabled = true;
                    }));
                    break;
                  } else {
                    Invoke(new MethodInvoker(() => {
                      informationLabel.Text = $"Scan {CurrentScan + 1} in progress...";
                      informationLabel.ForeColor = System.Drawing.Color.Green;
                      for (int n = 0; n < SamplesCount; n++) pSignal[n] = Signal[n] / CurrentScan;
                      UpdatePlot();
                    }));
                  }
                }
              }
            }
          }
          Console.WriteLine($"On sample {CurrentSample} of scan {CurrentScan}");
        }
        TransferQeue.Free();
      });
//      for (int n = 0; n < SamplesCount; n++) pSignal[n] /= ScanCount;
   //   for (int n = 0; n < SamplesCount; n++) pSignal[n] += 0.001*Math.Sin(2*Math.PI*1958 * n / 50000.0);


      //      deltaT = 1 / pUSB_NMR.Samplerate;
      //      vector_signal = new double[pUSB_NMR.SamplesCount];
      //      grinding_signal = new double[pUSB_NMR.SamplesCount];


        //Empaqueto los comandos, los mando al EP0

        //        bool go = false;
        //            if (runningTask == null)
        //            {
        //                try
        //                {
        //                    button1.BackColor = System.Drawing.Color.Orange;

        //                    await System.Threading.Tasks.Task.Run(() =>
        //                    {
        //                            signal = analogInReader.ReadMultiSample(_efnmr.number_samples);
        //                            for (int i = 0; i < signal.Length; i++)
        //                            {
        //                                vector_signal[i] = vector_signal[i]+signal[0, i];// vector_signal[i];// +;
        //                            }
        //                            signal = null;
        //                            myTask.Dispose();
        //                            runningTask = null;

        //Invoke(new MethodInvoker(() => { 
        //                                _bandFilter = new BandpassFilterButterworthImplementation(_efnmr.low_frequency, _efnmr.high_frequency, 6, _efnmr.sample_rate);
        //                                //for (int i = 0; i < vector_signal.Length; i++)
        //                                //{
        //                                //    vector_signal[i] = _bandFilter.compute(vector_signal[i]);
        //                                //}
        //                                  pPlot.Clear();
        //                                pPlot.AddSignal(pSignal, sampleRate: pSamplerate);
        //                                  pPlot.AxisAuto();
        //                                  mainPlotScottPlot.Refresh();
        //                                adquisitionTimer.Enabled = false;
        //                                adquisitionTimer.Stop();
        //                                adquisitionProgressBar.Value = 100;// Convert.ToInt16(_efnmr.progressbar_step* 100) + 1; 
        //                                informationLabel.Text = "Recovery time activated";
        //                              }));                                                     
        //                            System.Threading.Thread.Sleep(_efnmr.recovery_time);
        //                            Invoke(new MethodInvoker(() =>
        //                            {
        //                                scansLabel.Text = "Scans done: " + Convert.ToString(j+1);
        //                            }));
        //      }
        //                        _efnmr.sp_arduino.Close();
        //                        
        //                    });
//      if (setBackGroundCheckBox.Checked) {
//        WriteToFile("background.nmr", pSignal);
        //                        _efnmr.original_signal = vector_signal;
//        BandpassFilterButterworthImplementation _bandFilter = new BandpassFilterButterworthImplementation(pLowFrequency, pHighFrequency, 6, pSamplerate);
//                                for (int i = 0; i < pSignal.Length; i++)
//                                {
//          pSignal[i] = _bandFilter.compute(pSignal[i]);
//                                }
        //
        //                        _efnmr.filtered_signal = vector_signal;
        //
        //                        System.Numerics.Complex[] spectrum = FftSharp.FFT.Forward(_efnmr.filtered_signal);
        //                        _efnmr.fft = FftSharp.FFT.Magnitude(spectrum);
        //                        _efnmr.frequecies = FftSharp.FFT.FrequencyScale(_efnmr.fft.Length, _efnmr.sample_rate);
        //                        plt.Clear();
        //                        plt.AddSignal(_efnmr.filtered_signal, sampleRate: _efnmr.sample_rate);
        //                        plt.AxisAuto();
        //                        mainPlotScottPlot.Refresh();
        //                        setBackGroundCheckBox.Checked = false;
        //                        fftFIDCheckBox.Enabled = true;
        //                        informationLabel.Text = "successfully measured background";
        //                        informationLabel.ForeColor = System.Drawing.Color.Black;                       
        //
//      } else {
//        WriteToFile("measurement.nmr", pSignal);
        //                        _efnmr.original_signal = vector_signal;
        //                        for (int i = 0; i < vector_signal.Length; i++)
        //                        {
        //                            grinding_signal[i] = vector_signal[i] - _efnmr.background[i];
        //                        }
        //                        _efnmr.grinding_signal = grinding_signal;
        //
//        BandpassFilterButterworthImplementation _bandFilter = new BandpassFilterButterworthImplementation(pLowFrequency, pHighFrequency, 6, pSamplerate);
//        for (int i = 0; i < pSignal.Length; i++)
//        {
//          pSignal[i] = _bandFilter.compute(pSignal[i]);
//        }
        //                                _efnmr.filtered_signal = _efnmr.grinding_signal;
//        System.Numerics.Complex[] spectrum = FftSharp.FFT.Forward(pSignal);
//                                System.Numerics.Complex[] spectrum_background = FftSharp.FFT.Forward(_efnmr.background);
//                                pFFT = FftSharp.FFT.Magnitude(spectrum);
        //                            fft_background = FftSharp.FFT.Magnitude(spectrum_background);
//                                pFrequecies = FftSharp.FFT.FrequencyScale(pFFT.Length, pSamplerate);
//                                pPlot.Clear();
//                                pPlot.AddSignal(pSignal, sampleRate: pSamplerate);
//                                pPlot.AxisAuto();
//                                mainPlotScottPlot.Refresh();
        //                        informationLabel.Text = "successfully measured done";
        //
//                            }
      //
      //                    button1.BackColor = SystemColors.Control;
      //                    vector_signal = null;
      //                    grinding_signal = null;
      //                    vector_signal = new double[_efnmr.number_samples];
      //                    grinding_signal = new double[_efnmr.number_samples];
      //
      //                }
      //                catch (DaqException exception)
      //                {
      //
      //                    MessageBox.Show(exception.Message);
      //                    runningTask = null;
      //                    myTask.Dispose();
      //                    adquisitionTimer.Enabled = false;
      //                    adquisitionTimer.Stop();
      //                    adquisitionProgressBar.Value = 0;

      //                }
      //            }     


    }
    private void UpdatePlot() {
      if (pSignal == null) return;
      if (fftFIDCheckBox.Checked == false) {
        pPlot.Clear();
        pPlot.XLabel("Time/s");
        pPlot.YLabel("Intensity/a.u.");
        pPlot.AddSignal(pSignal, sampleRate: pSamplerate);
        pPlot.AxisAuto();            
        mainPlotScottPlot.Refresh();
      } else {
        System.Numerics.Complex[] Spectrum = FftSharp.FFT.Forward(pSignal);
        double[] FFT = FftSharp.FFT.Magnitude(Spectrum);
        double[] Frequecies = FftSharp.FFT.FrequencyScale(FFT.Length, pSamplerate);


        if (pBackgroundNoise.Length == pSignal.Length) {
          System.Numerics.Complex[] SpectrumBackground = FftSharp.FFT.Forward(pBackgroundNoise);
          double[] Noise = FftSharp.FFT.Magnitude(SpectrumBackground);
          for (int i = 0; i < FFT.Length; i++) FFT[i] = FFT[i] - Noise[i];
        }
        double Max = 0;
        double Freq = 0;
        for (int n = 0; n < FFT.Length; n++) { 
          if(FFT[n] > Max) {
            Max = FFT[n];
            Freq = Frequecies[n];
          }
        }
        pPlot.Clear();
        pPlot.AddScatterLines(Frequecies, FFT);
        pPlot.YLabel("Intensity/a.u.");
        pPlot.XLabel("Frequency/Hz");
        pPlot.SetAxisLimits(Freq - (100000 * Freq) / 1000000, Freq + (100000 * Freq) / 1000000);
        mainPlotScottPlot.Refresh();
      }
    }
    private void fftFIDCheckBox_CheckedChanged(object sender, EventArgs e) {
      UpdatePlot();
    }
    private void Form1_FormClosing(object sender, FormClosingEventArgs e) {
    try {
//                pUSB_NMR.sp_arduino.Close();
//                runningTask = null;
//                if (myTask != null)
//                {
//                    myTask.Dispose();
//                }
//        adquisitionTimer.Enabled = false;
//        adquisitionTimer.Stop();
//        adquisitionProgressBar.Value = 0;
      } catch (Exception) {
      }       
    }
    private void Form1_FormClosed(object sender, FormClosedEventArgs e) {
      try {
//        pUSB_NMR.sp_arduino.Close();
//        runningTask = null;
//        if (myTask != null) {
//          myTask.Dispose();
//        }
//        adquisitionTimer.Enabled = false;
//        adquisitionTimer.Stop();
//        adquisitionProgressBar.Value = 0;
      } catch (Exception) {
      }
    }
    private void setBackgroundBtn_Click(object sender, EventArgs e) {
      pBackgroundNoise = new double[pSignal.Length];
      for(int n = 0; n < pSignal.Length; n++) pBackgroundNoise[n] = pSignal[n];
    }
  }
}
