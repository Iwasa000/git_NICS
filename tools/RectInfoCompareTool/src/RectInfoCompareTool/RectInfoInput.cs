using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RectInfoCompareTool
{
    public partial class RectInfoInput : UserControl
    {
        public RectInfoInput()
        {
            InitializeComponent();
        }

        public string Title
        {
            get => this.TitleLabel.Text;
            set => this.TitleLabel.Text = value;
        }

        public string RectInfo => this.RectInfoText.Text;

        /// <summary>
        /// 貼り付けボタンクリック
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void PasteButton_Click(object sender, EventArgs e)
        {
            this.RectInfoText.Text = Clipboard.GetText();
        }
    }
}
