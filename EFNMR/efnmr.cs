using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.IO.Ports;
namespace EFNMR
{
    internal class efnmr
    {

        public double[] background { get; set; }
        public double[] original_signal { get; set; }
        public double[] grinding_signal { get; set; }
        public double[] filtered_signal { get; set; }
        public double[] fft { get; set; }
        public double[] frequecies { get; set; }
        public double low_frequency { get; set; }
        public double high_frequency { get; set; }
        public int number_scans { get; set; }
        public int recovery_time { get; set; }
        public int pulse_power { get; set; }
        public int sample_rate { get; set; }
        public int number_samples { get; set; }
        public double adquisition_time { get; set; }
        public double progressbar_step { get; set; }
        public string device_name { get; set; }
        public string channel_name { get; set; }
        public SerialPort sp_arduino { get; set; }

        public static void WriteToFile(string filename, double[] array)
        {
            using (StreamWriter file = new StreamWriter(filename))
            {
                string json = JsonConvert.SerializeObject(array);
                file.Write(json);
            }
        }

        public static double[] ReadFromFile(string filename)
        {
            using (StreamReader file = new StreamReader(filename))
            {
                string json = file.ReadToEnd();
                return JsonConvert.DeserializeObject<double[]>(json);
            }
        }

        public static SerialPort CheckSerialPort()
        {
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                using (SerialPort sp = new SerialPort(port, 9600))
                {
                    try
                    {
                        sp.Open();
                        sp.Write("check");  // Send check command
                        System.Threading.Thread.Sleep(500);  // Wait for the Arduino to respond
                        string response = sp.ReadLine();

                        if (response == "Arduino\r")  // You need to implement this checking mechanism on the Arduino
                        {
                            return sp;
                        }
                    }                   
                    catch (UnauthorizedAccessException e)
                    {
                        Console.WriteLine("Error: Unauthorized Access to " + port);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("Error accessing " + port + ": " + e.Message);
                    }

                }
            }
            return null;
        }
    }
    public class LowpassFilterButterworthImplementation
    {
        protected LowpassFilterButterworthSection[] section;

        public LowpassFilterButterworthImplementation
               (double cutoffFrequencyHz, int numSections, double Fs)
        {
            this.section = new LowpassFilterButterworthSection[numSections];
            for (int i = 0; i < numSections; i++)
            {
                this.section[i] = new LowpassFilterButterworthSection
                                  (cutoffFrequencyHz, i + 1, numSections * 2, Fs);
            }
        }
        public double compute(double input)
        {
            double output = input;
            for (int i = 0; i < this.section.Length; i++)
            {
                output = this.section[i].compute(output);
            }
            return output;
        }
    }
    public class LowpassFilterButterworthSection
    {
        protected FIRFilterImplementation firFilter = new FIRFilterImplementation(3);
        protected IIRFilterImplementation iirFilter = new IIRFilterImplementation(2);

        protected double[] a = new double[3];
        protected double[] b = new double[2];
        protected double gain;

        public LowpassFilterButterworthSection
               (double cutoffFrequencyHz, double k, double n, double Fs)
        {
            // compute the fixed filter coefficients
            double omegac = 2.0 * Fs * Math.Tan(Math.PI * cutoffFrequencyHz / Fs);
            double zeta = -Math.Cos(Math.PI * (2.0 * k + n - 1.0) / (2.0 * n));

            // fir section
            this.a[0] = omegac * omegac;
            this.a[1] = 2.0 * omegac * omegac;
            this.a[2] = omegac * omegac;

            //iir section
            //normalize coefficients so that b0 = 1, 
            //and higher-order coefficients are scaled and negated
            double b0 = (4.0 * Fs * Fs) + (4.0 * Fs * zeta * omegac) + (omegac * omegac);
            this.b[0] = ((2.0 * omegac * omegac) - (8.0 * Fs * Fs)) / (-b0);
            this.b[1] = ((4.0 * Fs * Fs) -
                         (4.0 * Fs * zeta * omegac) + (omegac * omegac)) / (-b0);
            this.gain = 1.0 / b0;
        }

        public double compute(double input)
        {
            // compute the result as the cascade of the fir and iir filters
            return this.iirFilter.compute
                   (this.firFilter.compute(this.gain * input, this.a), this.b);
        }
    }

    public class HighpassFilterButterworthImplementation
    {
        protected HighpassFilterButterworthSection[] section;

        public HighpassFilterButterworthImplementation
        (double cutoffFrequencyHz, int numSections, double Fs)
        {
            this.section = new HighpassFilterButterworthSection[numSections];
            for (int i = 0; i < numSections; i++)
            {
                this.section[i] = new HighpassFilterButterworthSection
                (cutoffFrequencyHz, i + 1, numSections * 2, Fs);
            }
        }
        public double compute(double input)
        {
            double output = input;
            for (int i = 0; i < this.section.Length; i++)
            {
                output = this.section[i].compute(output);
            }
            return output;
        }
    }


    public class HighpassFilterButterworthSection
    {
        protected FIRFilterImplementation firFilter = new FIRFilterImplementation(3);
        protected IIRFilterImplementation iirFilter = new IIRFilterImplementation(2);

        protected double[] a = new double[3];
        protected double[] b = new double[2];
        protected double gain;

        public HighpassFilterButterworthSection(double cutoffFrequencyHz, double k, double n, double Fs)
        {
            // pre-warp omegac and invert it
            double omegac = 1.0 / (2.0 * Fs * Math.Tan(Math.PI * cutoffFrequencyHz / Fs));

            // compute zeta
            double zeta = -Math.Cos(Math.PI * (2.0 * k + n - 1.0) / (2.0 * n));

            // fir section
            this.a[0] = 4.0 * Fs * Fs;
            this.a[1] = -8.0 * Fs * Fs;
            this.a[2] = 4.0 * Fs * Fs;

            //iir section
            //normalize coefficients so that b0 = 1
            //and higher-order coefficients are scaled and negated
            double b0 = (4.0 * Fs * Fs) + (4.0 * Fs * zeta / omegac) + (1.0 / (omegac * omegac));
            this.b[0] = ((2.0 / (omegac * omegac)) - (8.0 * Fs * Fs)) / (-b0);
            this.b[1] = ((4.0 * Fs * Fs)
                       - (4.0 * Fs * zeta / omegac) + (1.0 / (omegac * omegac))) / (-b0);
            this.gain = 1.0 / b0;
        }

        public double compute(double input)
        {
            // compute the result as the cascade of the fir and iir filters
            return this.iirFilter.compute
                (this.firFilter.compute(this.gain * input, this.a), this.b);
        }
    }

    public class BandpassFilterButterworthImplementation
    {
        protected LowpassFilterButterworthImplementation lowpassFilter;
        protected HighpassFilterButterworthImplementation highpassFilter;

        public BandpassFilterButterworthImplementation
           (double bottomFrequencyHz, double topFrequencyHz, int numSections, double Fs)
        {
            this.lowpassFilter = new LowpassFilterButterworthImplementation
                                 (topFrequencyHz, numSections, Fs);
            this.highpassFilter = new HighpassFilterButterworthImplementation
                                  (bottomFrequencyHz, numSections, Fs);
        }

        public double compute(double input)
        {
            // compute the result as the cascade of the highpass and lowpass filters
            return this.highpassFilter.compute(this.lowpassFilter.compute(input));
        }
    }
    public class FIRFilterImplementation
    {
        protected double[] z;
        public FIRFilterImplementation(int order)
        {
            this.z = new double[order];
        }

        public double compute(double input, double[] a)
        {
            // computes y(t) = a0*x(t) + a1*x(t-1) + a2*x(t-2) + ... an*x(t-n)
            double result = 0;

            for (int t = a.Length - 1; t >= 0; t--)
            {
                if (t > 0)
                {
                    this.z[t] = this.z[t - 1];
                }
                else
                {
                    this.z[t] = input;
                }
                result += a[t] * this.z[t];
            }
            return result;
        }
    }
    public class IIRFilterImplementation
    {
        protected double[] z;
        public IIRFilterImplementation(int order)
        {
            this.z = new double[order];
        }

        public double compute(double input, double[] a)
        {
            // computes y(t) = x(t) + a1*y(t-1) + a2*y(t-2) + ... an*y(t-n)
            // z-transform: H(z) = 1 / (1 - sum(1 to n) [an * y(t-n)])
            // a0 is assumed to be 1
            // y(t) is not stored, so y(t-1) is stored at z[0], 
            // and a1 is stored as coefficient[0]

            double result = input;

            for (int t = 0; t < a.Length; t++)
            {
                result += a[t] * this.z[t];
            }
            for (int t = a.Length - 1; t >= 0; t--)
            {
                if (t > 0)
                {
                    this.z[t] = this.z[t - 1];
                }
                else
                {
                    this.z[t] = result;
                }
            }
            return result;
        }
    }
}
