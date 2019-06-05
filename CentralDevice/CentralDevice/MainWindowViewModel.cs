using System;
using System.IO.Ports;
using System.Text;
using System.Windows;
using System.Windows.Input;

namespace CentralDevice
{
    class MainWindowViewModel : BaseViewModel
    {
        #region フィールド
        readonly SerialPort _serialPort = null;
        bool _shouldMoveToBottom = false;
        string _receivedMessage;
        DelegateCommand _openSerialPortCommand;
        DelegateCommand _closeSerialPortCommand;
        #endregion

        public MainWindowViewModel()
        {
            _serialPort = new SerialPort();
            _serialPort.DataReceived += new SerialDataReceivedEventHandler(OnDataReceived);
        }

        #region プロパティ
        public string SerialPortName { get; set; }

        public string ReceivedMessage
        {
            get { return _receivedMessage; }
            set
            {
                _receivedMessage = value;
                OnPropertyChanged();
                ShouldMoveToBottom = true;
            }
        }

        public ICommand OpenSerialPortCommand
        {
            get
            {
                return _openSerialPortCommand
                    ?? (_openSerialPortCommand = new DelegateCommand(x => OpenSerialPort(), x => !IsSerialPortOpened));
            }
        }

        public ICommand CloseSerialPortCommand
        {
            get
            {
                return _closeSerialPortCommand
                    ?? (_closeSerialPortCommand = new DelegateCommand(x => CloseSerialPort(), x => IsSerialPortOpened));
            }
        }

        public bool IsSerialPortOpened
        {
            get { return _serialPort.IsOpen; }
        }

        /// <summary>
        /// このプロパティをtrueにしたとき，MainWindowのシリアルモニター(中央のテキストボックス)の
        /// 表示領域を末尾に移動させる
        /// </summary>
        public bool ShouldMoveToBottom
        {
            get { return _shouldMoveToBottom; }
            set { _shouldMoveToBottom = value; OnPropertyChanged(); }
        }
        #endregion

        #region メソッド
        private void OpenSerialPort()
        {
            if (_serialPort.IsOpen)
            {
                MessageBox.Show("指定されたシリアルポートはすでに使用されています");
                return;
            }

            try
            {
                // ポート番号文字列=PortName, 115200bps/N81/ハードウェアフロー/UTF8/改行=CR
                _serialPort.PortName = SerialPortName;
                _serialPort.BaudRate = 115200;
                _serialPort.Parity = Parity.None;
                _serialPort.DataBits = 8;
                _serialPort.StopBits = StopBits.One;
                _serialPort.Handshake = Handshake.RequestToSend;
                _serialPort.Encoding = Encoding.UTF8;
                _serialPort.NewLine = "\n";
                _serialPort.WriteTimeout = 100000;
                _serialPort.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void CloseSerialPort()
        {
            // シリアルポートが既に閉じていれば，即退出
            if (!_serialPort.IsOpen)
                return;

            try
            {
                _serialPort.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        public void OnDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (_serialPort.IsOpen == false) return;

            while (_serialPort.BytesToRead > 0)
            {
                try
                {
                    ReceivedMessage += _serialPort.ReadLine();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }
        #endregion
    }
}
