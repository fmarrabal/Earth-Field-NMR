using OxyPlot;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using NationalInstruments.DAQmx;
using NationalInstruments;
using System.Runtime.InteropServices;
using ScottPlot;
using System.IO;
using FftSharp;
using MathNet.Filtering;
using MathNet.Numerics;
using MathNet.Filtering.FIR;
using System.Security.Cryptography;
using System.Runtime.InteropServices.ComTypes;
using System.IO.Ports;
namespace EFNMR
{
    public partial class Form1 : Form
    {
        private string filename = "background_nuevo.nmr";
        private NationalInstruments.DAQmx.Task myTask;            
        private NationalInstruments.DAQmx.Task runningTask;       
        private int numberOfSamples = 0;
        private AnalogEdgeStartTriggerSlope analogSlope = AnalogEdgeStartTriggerSlope.Rising;
        private AnalogMultiChannelReader analogInReader;
        private efnmr _efnmr = new efnmr();
        private double[,] signal;
        private double[] vector_signal;
        private double[] grinding_signal;
        private double deltaT;
        private double time;
        private BandpassFilterButterworthImplementation _bandFilter;
        string response = "nothing";
        double[] fft_background;
        ScottPlot.Plot plt = new ScottPlot.Plot();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {           
            plt = mainPlotScottPlot.Plot;
            plt.Style(Style.Seaborn);
            plt.Title("Earth Field NMR spectrometer");
            plt.XLabel("Time/s");
            plt.YLabel("Intensity/a.u.");
            plt.AxisAuto();
            mainPlotScottPlot.Refresh();
            physicalChannelComboBox.Items.AddRange(DaqSystem.Local.GetPhysicalChannels(PhysicalChannelTypes.AI, PhysicalChannelAccess.External));
            if (physicalChannelComboBox.Items.Count > 0)
                physicalChannelComboBox.SelectedIndex = 0;
            _efnmr.device_name = "Dev1";
            _efnmr.channel_name = "ai0";
            _efnmr.number_scans = Convert.ToInt32(triggerSourceTextBox.Text);
            _efnmr.sample_rate = Convert.ToInt32(rateNumeric.Value);
            _efnmr.number_samples = Convert.ToInt32(samplesPerChannelNumeric.Value);
            _efnmr.pulse_power = Convert.ToInt32(pulsePowerTextBox.Text);
            _efnmr.recovery_time = Convert.ToInt32(recoveryTimeTextBox.Text);
            _efnmr.adquisition_time  = Convert.ToInt32(adquisitionTimeTextBox.Text);
            _efnmr.progressbar_step = 100/(_efnmr.adquisition_time / adquisitionTimer.Interval);
            _efnmr.low_frequency = 1800;
            _efnmr.high_frequency = 2200;
            _efnmr.sp_arduino = efnmr.CheckSerialPort();
            _efnmr.sp_arduino.DataReceived += SerialPortDataReceived;
            if (File.Exists(filename))
            {
                _efnmr.background = efnmr.ReadFromFile(filename);

            }
            else
            {
                informationLabel.Text = "Please, set background nmr signal";
                informationLabel.ForeColor = System.Drawing.Color.Red;
                setBackGroundCheckBox.Checked = true;
                fftFIDCheckBox.Enabled = false ;
            }

            deltaT = 1 / _efnmr.sample_rate;
            vector_signal = new double[_efnmr.number_samples];
            grinding_signal = new double[_efnmr.number_samples];
            set_aq(adquisitionTimeTextBox.Text);
        }
        void set_aq(string aq)
        {
            _efnmr.sp_arduino.Open();
            _efnmr.sp_arduino.Write("set_aq");
            System.Threading.Thread.Sleep(1000);
            _efnmr.sp_arduino.Write("OK");
            System.Threading.Thread.Sleep(1000);
            _efnmr.sp_arduino.Write(aq);
            System.Threading.Thread.Sleep(1000);
            
        }
        void SerialPortDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
             response = _efnmr.sp_arduino.ReadLine();
            Invoke(new MethodInvoker(() => { responseLabel.Text = response; }));
            //_efnmr.sp_arduino.Write("OK");
        }
        private async void button1_Click(object sender, EventArgs e)
        {
            
            bool go = false;
            if (runningTask == null)
            {
                try
                {
                    button1.BackColor = System.Drawing.Color.Orange;

                    await System.Threading.Tasks.Task.Run(() =>
                    {
                        Invoke(new MethodInvoker(() =>
                        {
                            scansLabel.Text = "Scans done: 0";
                        }));
                        try
                        {
                            _efnmr.sp_arduino.Close();
                        }
                        catch (Exception) { }


                        _efnmr.sp_arduino.Open();
                        

                        System.Threading.Thread.Sleep(1000);

                        _efnmr.sp_arduino.Write("set_scans");
                        System.Threading.Thread.Sleep(1000);
                        _efnmr.sp_arduino.Write("OK");
                        System.Threading.Thread.Sleep(1000);
                        _efnmr.sp_arduino.Write(triggerSourceTextBox.Text);
                        System.Threading.Thread.Sleep(1000);
                        //System.Threading.Thread.Sleep(1000);

                        for (int j = 0; j < _efnmr.number_scans; j++)
                        {

                            Invoke(new MethodInvoker(() => { informationLabel.Text = "Polarization coil activated"; }));
                            myTask = new NationalInstruments.DAQmx.Task();
                            myTask.AIChannels.CreateVoltageChannel($"{_efnmr.device_name}/{_efnmr.channel_name}", "",
                                (AITerminalConfiguration)(-1), Convert.ToDouble(minimumValueNumeric.Value),
                                Convert.ToDouble(maximumValueNumeric.Value), AIVoltageUnits.Volts);
                            myTask.Timing.ConfigureSampleClock("", _efnmr.sample_rate, SampleClockActiveEdge.Rising, SampleQuantityMode.ContinuousSamples, _efnmr.number_samples);
                            myTask.Control(TaskAction.Verify);
                            runningTask = myTask;

                            _efnmr.sp_arduino.Write("perform");
                            //System.Threading.Thread.Sleep(1000);
                           

                            while (!go)
                            {

                                if (response == "START_DAQ\r")
                                {
                                    _efnmr.sp_arduino.Write("OK");
                                    Invoke(new MethodInvoker(() => { informationLabel.Text = "Polarization coil deactivated"; }));
                                    
                                    go = true;
                                }
                            }

                            System.Threading.Thread.Sleep(700);
                            go = false;
                            Invoke(new MethodInvoker(() => { informationLabel.Text = "DAQ activated"; }));


                            analogInReader = new AnalogMultiChannelReader(myTask.Stream);
                         
                            Invoke(new MethodInvoker(() =>
                            {
                                adquisitionProgressBar.Minimum = 0;
                                adquisitionProgressBar.Maximum = 100;// Convert.ToInt16(_efnmr.progressbar_step*100)+1;
                                adquisitionProgressBar.Value = 0;
                                adquisitionTimer.Interval = 100;
                                adquisitionTimer.Enabled = true;
                                adquisitionTimer.Start();
                            }));
                                                                            
                            signal = analogInReader.ReadMultiSample(_efnmr.number_samples);
                            
                            System.Threading.Thread.Sleep(100);
                            Invoke(new MethodInvoker(() =>
                            {
                                informationLabel.Text = "DAQ deactivated";
                            }));
                            for (int i = 0; i < signal.Length; i++)
                            {
                                vector_signal[i] = vector_signal[i]+signal[0, i];// vector_signal[i];// +;
                            }
                            signal = null;
                            myTask.Dispose();
                            runningTask = null;
                                                          
                            Invoke(new MethodInvoker(() =>
                            {
                                _bandFilter = new BandpassFilterButterworthImplementation(_efnmr.low_frequency, _efnmr.high_frequency, 6, _efnmr.sample_rate);
                                //for (int i = 0; i < vector_signal.Length; i++)
                                //{
                                //    vector_signal[i] = _bandFilter.compute(vector_signal[i]);
                                //}
                                plt.Clear();
                                plt.AddSignal(vector_signal, sampleRate: _efnmr.sample_rate);
                                plt.AxisAuto();
                                mainPlotScottPlot.Refresh();
                                adquisitionTimer.Enabled = false;
                                adquisitionTimer.Stop();
                                adquisitionProgressBar.Value = 100;// Convert.ToInt16(_efnmr.progressbar_step* 100) + 1; 
                                informationLabel.Text = "Recovery time activated";
                            }));                                                     
                            System.Threading.Thread.Sleep(_efnmr.recovery_time);
                            Invoke(new MethodInvoker(() =>
                            {
                                scansLabel.Text = "Scans done: " + Convert.ToString(j+1);
                            }));

                        }
                        _efnmr.sp_arduino.Close();
                        
                    });
                    if (setBackGroundCheckBox.Checked)
                    {

                        efnmr.WriteToFile("background_nuevo.nmr", vector_signal);
                        _efnmr.original_signal = vector_signal;
                        _bandFilter = new BandpassFilterButterworthImplementation(_efnmr.low_frequency, _efnmr.high_frequency, 6, _efnmr.sample_rate);

                        for (int i = 0; i < vector_signal.Length; i++)
                        {
                            vector_signal[i] = _bandFilter.compute(vector_signal[i]);
                        }

                        _efnmr.filtered_signal = vector_signal;

                        System.Numerics.Complex[] spectrum = FftSharp.FFT.Forward(_efnmr.filtered_signal);
                        _efnmr.fft = FftSharp.FFT.Magnitude(spectrum);
                        _efnmr.frequecies = FftSharp.FFT.FrequencyScale(_efnmr.fft.Length, _efnmr.sample_rate);
                        plt.Clear();
                        plt.AddSignal(_efnmr.filtered_signal, sampleRate: _efnmr.sample_rate);
                        plt.AxisAuto();
                        mainPlotScottPlot.Refresh();
                        setBackGroundCheckBox.Checked = false;
                        fftFIDCheckBox.Enabled = true;
                        informationLabel.Text = "successfully measured background";
                        informationLabel.ForeColor = System.Drawing.Color.Black;                       

                    }
                    else
                    {
                        efnmr.WriteToFile("measurement_nuevo.nmr", vector_signal);
                        _efnmr.original_signal = vector_signal;
                        for (int i = 0; i < vector_signal.Length; i++)
                        {
                            grinding_signal[i] = vector_signal[i] - _efnmr.background[i];
                        }
                        _efnmr.grinding_signal = grinding_signal;

                        _bandFilter = new BandpassFilterButterworthImplementation(_efnmr.low_frequency, _efnmr.high_frequency, 6, _efnmr.sample_rate);
                        for (int i = 0; i < vector_signal.Length; i++)
                        {
                            _efnmr.grinding_signal[i] = _bandFilter.compute(_efnmr.original_signal[i]);
                        }
                        _efnmr.filtered_signal = _efnmr.grinding_signal;
                        System.Numerics.Complex[] spectrum = FftSharp.FFT.Forward(_efnmr.filtered_signal);
                        System.Numerics.Complex[] spectrum_background = FftSharp.FFT.Forward(_efnmr.background);
                        _efnmr.fft = FftSharp.FFT.Magnitude(spectrum);
                            fft_background = FftSharp.FFT.Magnitude(spectrum_background);
                        _efnmr.frequecies = FftSharp.FFT.FrequencyScale(_efnmr.fft.Length, _efnmr.sample_rate);
                        plt.Clear();
                        plt.AddSignal(_efnmr.filtered_signal, sampleRate: _efnmr.sample_rate);
                        plt.AxisAuto();
                        mainPlotScottPlot.Refresh();
                        informationLabel.Text = "successfully measured done";

                    }

                    button1.BackColor = SystemColors.Control;
                    vector_signal = null;
                    grinding_signal = null;
                    vector_signal = new double[_efnmr.number_samples];
                    grinding_signal = new double[_efnmr.number_samples];

                }
                catch (DaqException exception)
                {

                    MessageBox.Show(exception.Message);
                    runningTask = null;
                    myTask.Dispose();
                    adquisitionTimer.Enabled = false;
                    adquisitionTimer.Stop();
                    adquisitionProgressBar.Value = 0;

                }
            }     
        }

        private void adquisitionTimer_Tick(object sender, EventArgs e)
        {

            adquisitionProgressBar.Value = adquisitionProgressBar.Value +1;

        }

        private void fftFIDCheckBox_CheckedChanged(object sender, EventArgs e)
        {

            if (fftFIDCheckBox.Checked)
            {
                plt.Clear();
                plt.XLabel("Time/s");
                plt.YLabel("Intensity/a.u.");
                plt.AddSignal(_efnmr.filtered_signal, sampleRate: _efnmr.sample_rate);
                plt.AxisAuto();            
                mainPlotScottPlot.Refresh();
            }
            else
            {
                double[] fft_grind = new double[fft_background.Length];


                for (int i = 0; i < fft_grind.Length; i++)
                {
                    fft_grind[i] = _efnmr.fft[i] - fft_grind[i];
                }

                                   plt.Clear();
                plt.AddScatterLines(_efnmr.frequecies, fft_grind);
                plt.YLabel("Intensity/a.u.");
                plt.XLabel("Frequency/Hz");
                plt.SetAxisLimits(_efnmr.low_frequency, _efnmr.high_frequency, 0, 1);
                mainPlotScottPlot.Refresh();
            }

        }

        private void triggerSourceTextBox_TextChanged(object sender, EventArgs e)
        {
            try
            {
                _efnmr.number_scans = Convert.ToInt32(triggerSourceTextBox.Text);
            }
            catch (Exception)
            {

                
            }
            
        }

        private void physicalChannelComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
           
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                _efnmr.sp_arduino.Close();
                runningTask = null;
                if (myTask != null)
                {
                    myTask.Dispose();
                }
                
                
                adquisitionTimer.Enabled = false;
                adquisitionTimer.Stop();
                adquisitionProgressBar.Value = 0;
            }
            catch (Exception)
            {

               
            }
           
        }

        private void physicalChannelComboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            try
            {
                string s = physicalChannelComboBox.Text;
                string[] subs = s.Split('/');
                _efnmr.device_name = subs[0];
                _efnmr.channel_name = subs[1];
            }
            catch (Exception)
            {

               
            }
            
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                _efnmr.sp_arduino.Close();
                runningTask = null;
                if (myTask != null)
                {
                    myTask.Dispose();
                }


                adquisitionTimer.Enabled = false;
                adquisitionTimer.Stop();
                adquisitionProgressBar.Value = 0;
            }
            catch (Exception)
            {


            }
        }

        private void setBackGroundCheckBox_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
