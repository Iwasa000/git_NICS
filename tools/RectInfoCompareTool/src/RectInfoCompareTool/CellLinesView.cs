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
    public partial class CellLinesView : UserControl
    {
        /// <summary>セル番号</summary>
        private int m_cellNumber = 0;

        public CellLinesView()
        {
            InitializeComponent();
        }

        public int CellNumber
        {
            get => m_cellNumber;
            set
            {
                m_cellNumber = value;
                this.TitleLabel.Text = "セル" + m_cellNumber.ToString("00");
            }
        }

        public string Lines
        {
            get => this.LinesText.Text;
            set => this.LinesText.Text = value;
        }

        /// <summary>
        /// コピーボタンクリック
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CopyButton_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(this.LinesText.Text);
        }
    }
}
