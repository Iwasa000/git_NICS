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
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 比較ボタンのクリックイベント処理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CompareButton_Click(object sender, EventArgs e)
        {
            try
            {
                this.CompareSummaryText.ForeColor = Color.Black;
                this.CellsSettingListPanel.Controls.Clear();

                var comparer = new RectInfoComparer(GetCompareSettings());
                var result = comparer.CompareLines(GetRectInfos());
                SetCompareResult(result);
            }
            catch (Exception ex)
            {
                this.CompareSummaryText.ForeColor = Color.Red;
                this.CompareSummaryText.Text = ex.ToString();
            }
        }

        /// <summary>
        /// 比較設定を取得する
        /// </summary>
        /// <returns></returns>
        private RectInfoCompareSettings GetCompareSettings()
        {
            var setting = new RectInfoCompareSettings()
            {
                LineCellCount = (int)this.LineCellCount.Value,
                IsCheckHeight = this.CheckHeight.Checked,
                IsCheckWidth = this.CheckWidth.Checked,
                IsCheckFontName = this.CheckFontName.Checked,
                IsCheckFontSize = this.CheckFontSize.Checked,
                IsCheckBorderDisappears = this.CheckBorderDisappears.Checked,
            };

            return setting;
        }

        /// <summary>
        /// RectInfo文字列のリストを取得する
        /// </summary>
        /// <returns></returns>
        private IList<string> GetRectInfos()
        {
            return new List<string>
            {
                this.RectInfoInput0.RectInfo,
                this.RectInfoInput1.RectInfo,
                this.RectInfoInput2.RectInfo,
                this.RectInfoInput3.RectInfo,
            };
        }

        /// <summary>
        /// 比較結果を画面にセットする
        /// </summary>
        /// <param name="result"></param>
        private void SetCompareResult(RectInfoCompareResult result)
        {
            SetCompareSummary(result);
            AddCellLinesView(result);
        }

        /// <summary>
        /// 比較サマリーを設定する
        /// </summary>
        /// <param name="result"></param>
        private void SetCompareSummary(RectInfoCompareResult result)
        {
            if (result.HasError)
            {
                this.CompareSummaryText.Text = "比較対象項目が異なるセルが存在します。";
                foreach (int cellI in result.ErrorCellIndexes)
                {
                    foreach (var errorMessage in result.ErrorMessages[cellI])
                    {
                        this.CompareSummaryText.AppendText(Environment.NewLine);
                        this.CompareSummaryText.AppendText($"セル{(cellI + 1).ToString("00")}: {errorMessage}");
                    }
                }
            }
            else
            {
                this.CompareSummaryText.Text = "全てのセルで比較対象項目が一致しました。";
            }
        }

        /// <summary>
        /// 各セルの行設定リスト表示コントロールを追加する
        /// </summary>
        /// <param name="result"></param>
        private void AddCellLinesView(RectInfoCompareResult result)
        {
            for (int ci = result.CellSettingLines.Count - 1; ci >= 0; ci--)
            {
                var view = new CellLinesView();
                view.Dock = DockStyle.Top;
                view.CellNumber = ci + 1;
                view.Lines = result.CellSettingLines[ci];
                this.CellsSettingListPanel.Controls.Add(view);
            }
        }
    }
}
