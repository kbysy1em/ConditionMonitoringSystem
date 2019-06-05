using System.Windows;
using System.Windows.Controls;

namespace CentralDevice
{
    /// <summary>
    /// テキストボックスの表示領域を末尾に移動させるための添付ビヘイビア
    /// </summary>
    class MoveToBottomAttachedBehavior
    {
        public static DependencyProperty MoveToBottomProperty = DependencyProperty.RegisterAttached(
            "MoveToBottom", typeof(bool), typeof(MoveToBottomAttachedBehavior), new FrameworkPropertyMetadata
            {
                DefaultValue = false,
                PropertyChangedCallback = OnMoveToBottomChanged,
                BindsTwoWayByDefault = true
            });

        public static bool GetMoveToBottom(DependencyObject obj)
        {
            return (bool)obj.GetValue(MoveToBottomProperty);
        }

        public static void SetMoveToBottom(DependencyObject obj, bool value)
        {
            obj.SetValue(MoveToBottomProperty, value);
        }

        private static void OnMoveToBottomChanged(DependencyObject o, DependencyPropertyChangedEventArgs e)
        {
            var textBox = o as TextBox;
            if (textBox == null)
                return;

            if ((bool)e.NewValue == false)
                return;

            textBox.Select(textBox.Text.Length, 0);
            //textBox.Focus();
            textBox.ScrollToEnd();
            SetMoveToBottom(textBox, false);
        }
    }
}
