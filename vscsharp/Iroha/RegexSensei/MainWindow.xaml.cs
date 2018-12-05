using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RegexSensei {
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window {
        public MainWindow() {
            InitializeComponent();
        }

        private void ToggleButton_Loaded(object sender, RoutedEventArgs e) {
            var button = sender as ToggleButton;

            button.SetBinding(ToggleButton.IsCheckedProperty, new Binding("IsOpen") {
                Source = button.ContextMenu
            });
            button.ContextMenu.PlacementTarget = button;
            button.ContextMenu.Placement= PlacementMode.Bottom;
        }
    }
}
