
namespace RectInfoCompareTool
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.SettingsPanel = new System.Windows.Forms.Panel();
            this.CheckFontSize = new System.Windows.Forms.CheckBox();
            this.CheckFontName = new System.Windows.Forms.CheckBox();
            this.CheckWidth = new System.Windows.Forms.CheckBox();
            this.CheckHeight = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.LineCellCount = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.BodyPanel = new System.Windows.Forms.Panel();
            this.CompareResultPanel = new System.Windows.Forms.Panel();
            this.CellsSettingListOwnerPanel = new System.Windows.Forms.Panel();
            this.CellsSettingListPanel = new System.Windows.Forms.Panel();
            this.label7 = new System.Windows.Forms.Label();
            this.CompareSummaryPanel = new System.Windows.Forms.Panel();
            this.CompareSummaryText = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.CheckTargetPanel = new System.Windows.Forms.Panel();
            this.RectInfoInput3 = new RectInfoCompareTool.RectInfoInput();
            this.CompareButton = new System.Windows.Forms.Button();
            this.RectInfoInput2 = new RectInfoCompareTool.RectInfoInput();
            this.RectInfoInput1 = new RectInfoCompareTool.RectInfoInput();
            this.RectInfoInput0 = new RectInfoCompareTool.RectInfoInput();
            this.label1 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.CheckBorderDisappears = new System.Windows.Forms.CheckBox();
            this.SettingsPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.LineCellCount)).BeginInit();
            this.BodyPanel.SuspendLayout();
            this.CompareResultPanel.SuspendLayout();
            this.CellsSettingListOwnerPanel.SuspendLayout();
            this.CompareSummaryPanel.SuspendLayout();
            this.CheckTargetPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // SettingsPanel
            // 
            this.SettingsPanel.BackColor = System.Drawing.Color.AliceBlue;
            this.SettingsPanel.Controls.Add(this.CheckBorderDisappears);
            this.SettingsPanel.Controls.Add(this.label8);
            this.SettingsPanel.Controls.Add(this.CheckFontSize);
            this.SettingsPanel.Controls.Add(this.CheckFontName);
            this.SettingsPanel.Controls.Add(this.CheckWidth);
            this.SettingsPanel.Controls.Add(this.CheckHeight);
            this.SettingsPanel.Controls.Add(this.label3);
            this.SettingsPanel.Controls.Add(this.LineCellCount);
            this.SettingsPanel.Controls.Add(this.label2);
            this.SettingsPanel.Dock = System.Windows.Forms.DockStyle.Left;
            this.SettingsPanel.Location = new System.Drawing.Point(0, 0);
            this.SettingsPanel.Name = "SettingsPanel";
            this.SettingsPanel.Padding = new System.Windows.Forms.Padding(3);
            this.SettingsPanel.Size = new System.Drawing.Size(229, 589);
            this.SettingsPanel.TabIndex = 0;
            // 
            // CheckFontSize
            // 
            this.CheckFontSize.AutoSize = true;
            this.CheckFontSize.Checked = true;
            this.CheckFontSize.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CheckFontSize.Dock = System.Windows.Forms.DockStyle.Top;
            this.CheckFontSize.Location = new System.Drawing.Point(3, 133);
            this.CheckFontSize.Name = "CheckFontSize";
            this.CheckFontSize.Padding = new System.Windows.Forms.Padding(6, 0, 0, 0);
            this.CheckFontSize.Size = new System.Drawing.Size(223, 20);
            this.CheckFontSize.TabIndex = 7;
            this.CheckFontSize.Text = "フォントサイズ";
            this.CheckFontSize.UseVisualStyleBackColor = true;
            // 
            // CheckFontName
            // 
            this.CheckFontName.AutoSize = true;
            this.CheckFontName.Checked = true;
            this.CheckFontName.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CheckFontName.Dock = System.Windows.Forms.DockStyle.Top;
            this.CheckFontName.Location = new System.Drawing.Point(3, 113);
            this.CheckFontName.Name = "CheckFontName";
            this.CheckFontName.Padding = new System.Windows.Forms.Padding(6, 0, 0, 0);
            this.CheckFontName.Size = new System.Drawing.Size(223, 20);
            this.CheckFontName.TabIndex = 6;
            this.CheckFontName.Text = "フォント名";
            this.CheckFontName.UseVisualStyleBackColor = true;
            // 
            // CheckWidth
            // 
            this.CheckWidth.AutoSize = true;
            this.CheckWidth.Checked = true;
            this.CheckWidth.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CheckWidth.Dock = System.Windows.Forms.DockStyle.Top;
            this.CheckWidth.Location = new System.Drawing.Point(3, 93);
            this.CheckWidth.Name = "CheckWidth";
            this.CheckWidth.Padding = new System.Windows.Forms.Padding(6, 0, 0, 0);
            this.CheckWidth.Size = new System.Drawing.Size(223, 20);
            this.CheckWidth.TabIndex = 5;
            this.CheckWidth.Text = "幅";
            this.CheckWidth.UseVisualStyleBackColor = true;
            // 
            // CheckHeight
            // 
            this.CheckHeight.AutoSize = true;
            this.CheckHeight.Checked = true;
            this.CheckHeight.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CheckHeight.Dock = System.Windows.Forms.DockStyle.Top;
            this.CheckHeight.Location = new System.Drawing.Point(3, 73);
            this.CheckHeight.Name = "CheckHeight";
            this.CheckHeight.Padding = new System.Windows.Forms.Padding(6, 0, 0, 0);
            this.CheckHeight.Size = new System.Drawing.Size(223, 20);
            this.CheckHeight.TabIndex = 4;
            this.CheckHeight.Text = "高さ";
            this.CheckHeight.UseVisualStyleBackColor = true;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Top;
            this.label3.ForeColor = System.Drawing.Color.Blue;
            this.label3.Location = new System.Drawing.Point(3, 42);
            this.label3.Name = "label3";
            this.label3.Padding = new System.Windows.Forms.Padding(0, 10, 0, 5);
            this.label3.Size = new System.Drawing.Size(72, 31);
            this.label3.TabIndex = 3;
            this.label3.Text = "比較項目";
            // 
            // LineCellCount
            // 
            this.LineCellCount.Dock = System.Windows.Forms.DockStyle.Top;
            this.LineCellCount.Location = new System.Drawing.Point(3, 19);
            this.LineCellCount.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.LineCellCount.Name = "LineCellCount";
            this.LineCellCount.Size = new System.Drawing.Size(223, 23);
            this.LineCellCount.TabIndex = 1;
            this.LineCellCount.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Top;
            this.label2.Location = new System.Drawing.Point(3, 3);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(96, 16);
            this.label2.TabIndex = 0;
            this.label2.Text = "1行のセル数";
            // 
            // BodyPanel
            // 
            this.BodyPanel.Controls.Add(this.CompareResultPanel);
            this.BodyPanel.Controls.Add(this.CheckTargetPanel);
            this.BodyPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.BodyPanel.Location = new System.Drawing.Point(229, 0);
            this.BodyPanel.Name = "BodyPanel";
            this.BodyPanel.Size = new System.Drawing.Size(836, 589);
            this.BodyPanel.TabIndex = 1;
            // 
            // CompareResultPanel
            // 
            this.CompareResultPanel.BackColor = System.Drawing.Color.Linen;
            this.CompareResultPanel.Controls.Add(this.CellsSettingListOwnerPanel);
            this.CompareResultPanel.Controls.Add(this.CompareSummaryPanel);
            this.CompareResultPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CompareResultPanel.Location = new System.Drawing.Point(0, 153);
            this.CompareResultPanel.Name = "CompareResultPanel";
            this.CompareResultPanel.Size = new System.Drawing.Size(836, 436);
            this.CompareResultPanel.TabIndex = 4;
            // 
            // CellsSettingListOwnerPanel
            // 
            this.CellsSettingListOwnerPanel.Controls.Add(this.CellsSettingListPanel);
            this.CellsSettingListOwnerPanel.Controls.Add(this.label7);
            this.CellsSettingListOwnerPanel.Dock = System.Windows.Forms.DockStyle.Right;
            this.CellsSettingListOwnerPanel.Location = new System.Drawing.Point(449, 0);
            this.CellsSettingListOwnerPanel.Name = "CellsSettingListOwnerPanel";
            this.CellsSettingListOwnerPanel.Padding = new System.Windows.Forms.Padding(3);
            this.CellsSettingListOwnerPanel.Size = new System.Drawing.Size(387, 436);
            this.CellsSettingListOwnerPanel.TabIndex = 1;
            // 
            // CellsSettingListPanel
            // 
            this.CellsSettingListPanel.AutoScroll = true;
            this.CellsSettingListPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CellsSettingListPanel.Location = new System.Drawing.Point(3, 22);
            this.CellsSettingListPanel.Name = "CellsSettingListPanel";
            this.CellsSettingListPanel.Size = new System.Drawing.Size(381, 411);
            this.CellsSettingListPanel.TabIndex = 1;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Dock = System.Windows.Forms.DockStyle.Top;
            this.label7.Location = new System.Drawing.Point(3, 3);
            this.label7.Name = "label7";
            this.label7.Padding = new System.Windows.Forms.Padding(0, 0, 0, 3);
            this.label7.Size = new System.Drawing.Size(104, 19);
            this.label7.TabIndex = 0;
            this.label7.Text = "セル別の設定";
            // 
            // CompareSummaryPanel
            // 
            this.CompareSummaryPanel.Controls.Add(this.CompareSummaryText);
            this.CompareSummaryPanel.Controls.Add(this.label6);
            this.CompareSummaryPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CompareSummaryPanel.Location = new System.Drawing.Point(0, 0);
            this.CompareSummaryPanel.Name = "CompareSummaryPanel";
            this.CompareSummaryPanel.Padding = new System.Windows.Forms.Padding(3);
            this.CompareSummaryPanel.Size = new System.Drawing.Size(836, 436);
            this.CompareSummaryPanel.TabIndex = 0;
            // 
            // CompareSummaryText
            // 
            this.CompareSummaryText.AcceptsReturn = true;
            this.CompareSummaryText.BackColor = System.Drawing.Color.White;
            this.CompareSummaryText.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CompareSummaryText.Location = new System.Drawing.Point(3, 22);
            this.CompareSummaryText.Multiline = true;
            this.CompareSummaryText.Name = "CompareSummaryText";
            this.CompareSummaryText.ReadOnly = true;
            this.CompareSummaryText.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.CompareSummaryText.Size = new System.Drawing.Size(830, 411);
            this.CompareSummaryText.TabIndex = 1;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Dock = System.Windows.Forms.DockStyle.Top;
            this.label6.Location = new System.Drawing.Point(3, 3);
            this.label6.Name = "label6";
            this.label6.Padding = new System.Windows.Forms.Padding(0, 0, 0, 3);
            this.label6.Size = new System.Drawing.Size(72, 19);
            this.label6.TabIndex = 0;
            this.label6.Text = "比較結果";
            // 
            // CheckTargetPanel
            // 
            this.CheckTargetPanel.BackColor = System.Drawing.Color.Honeydew;
            this.CheckTargetPanel.Controls.Add(this.RectInfoInput3);
            this.CheckTargetPanel.Controls.Add(this.CompareButton);
            this.CheckTargetPanel.Controls.Add(this.RectInfoInput2);
            this.CheckTargetPanel.Controls.Add(this.RectInfoInput1);
            this.CheckTargetPanel.Controls.Add(this.RectInfoInput0);
            this.CheckTargetPanel.Controls.Add(this.label1);
            this.CheckTargetPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.CheckTargetPanel.Location = new System.Drawing.Point(0, 0);
            this.CheckTargetPanel.Name = "CheckTargetPanel";
            this.CheckTargetPanel.Padding = new System.Windows.Forms.Padding(3);
            this.CheckTargetPanel.Size = new System.Drawing.Size(836, 153);
            this.CheckTargetPanel.TabIndex = 3;
            // 
            // RectInfoInput3
            // 
            this.RectInfoInput3.Dock = System.Windows.Forms.DockStyle.Top;
            this.RectInfoInput3.Font = new System.Drawing.Font("BIZ UDゴシック", 12F);
            this.RectInfoInput3.Location = new System.Drawing.Point(3, 91);
            this.RectInfoInput3.Margin = new System.Windows.Forms.Padding(4);
            this.RectInfoInput3.Name = "RectInfoInput3";
            this.RectInfoInput3.Size = new System.Drawing.Size(830, 24);
            this.RectInfoInput3.TabIndex = 4;
            this.RectInfoInput3.Title = "RectInfo3";
            // 
            // CompareButton
            // 
            this.CompareButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CompareButton.Location = new System.Drawing.Point(755, 122);
            this.CompareButton.Name = "CompareButton";
            this.CompareButton.Size = new System.Drawing.Size(75, 23);
            this.CompareButton.TabIndex = 5;
            this.CompareButton.Text = "比較";
            this.CompareButton.UseVisualStyleBackColor = true;
            this.CompareButton.Click += new System.EventHandler(this.CompareButton_Click);
            // 
            // RectInfoInput2
            // 
            this.RectInfoInput2.Dock = System.Windows.Forms.DockStyle.Top;
            this.RectInfoInput2.Font = new System.Drawing.Font("BIZ UDゴシック", 12F);
            this.RectInfoInput2.Location = new System.Drawing.Point(3, 67);
            this.RectInfoInput2.Margin = new System.Windows.Forms.Padding(4);
            this.RectInfoInput2.Name = "RectInfoInput2";
            this.RectInfoInput2.Size = new System.Drawing.Size(830, 24);
            this.RectInfoInput2.TabIndex = 3;
            this.RectInfoInput2.Title = "RectInfo2";
            // 
            // RectInfoInput1
            // 
            this.RectInfoInput1.Dock = System.Windows.Forms.DockStyle.Top;
            this.RectInfoInput1.Font = new System.Drawing.Font("BIZ UDゴシック", 12F);
            this.RectInfoInput1.Location = new System.Drawing.Point(3, 43);
            this.RectInfoInput1.Margin = new System.Windows.Forms.Padding(4);
            this.RectInfoInput1.Name = "RectInfoInput1";
            this.RectInfoInput1.Size = new System.Drawing.Size(830, 24);
            this.RectInfoInput1.TabIndex = 2;
            this.RectInfoInput1.Title = "RectInfo1";
            // 
            // RectInfoInput0
            // 
            this.RectInfoInput0.Dock = System.Windows.Forms.DockStyle.Top;
            this.RectInfoInput0.Font = new System.Drawing.Font("BIZ UDゴシック", 12F);
            this.RectInfoInput0.Location = new System.Drawing.Point(3, 19);
            this.RectInfoInput0.Margin = new System.Windows.Forms.Padding(4);
            this.RectInfoInput0.Name = "RectInfoInput0";
            this.RectInfoInput0.Size = new System.Drawing.Size(830, 24);
            this.RectInfoInput0.TabIndex = 1;
            this.RectInfoInput0.Title = "RectInfo";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(3, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(104, 16);
            this.label1.TabIndex = 0;
            this.label1.Text = "チェック対象";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Dock = System.Windows.Forms.DockStyle.Top;
            this.label8.ForeColor = System.Drawing.Color.Blue;
            this.label8.Location = new System.Drawing.Point(3, 153);
            this.label8.Name = "label8";
            this.label8.Padding = new System.Windows.Forms.Padding(0, 10, 0, 5);
            this.label8.Size = new System.Drawing.Size(88, 31);
            this.label8.TabIndex = 8;
            this.label8.Text = "特殊な検証";
            // 
            // CheckBorderDisappears
            // 
            this.CheckBorderDisappears.AutoSize = true;
            this.CheckBorderDisappears.Checked = true;
            this.CheckBorderDisappears.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CheckBorderDisappears.Dock = System.Windows.Forms.DockStyle.Top;
            this.CheckBorderDisappears.Location = new System.Drawing.Point(3, 184);
            this.CheckBorderDisappears.Name = "CheckBorderDisappears";
            this.CheckBorderDisappears.Padding = new System.Windows.Forms.Padding(6, 0, 0, 0);
            this.CheckBorderDisappears.Size = new System.Drawing.Size(223, 20);
            this.CheckBorderDisappears.TabIndex = 9;
            this.CheckBorderDisappears.Text = "枠線が消える設定";
            this.CheckBorderDisappears.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1065, 589);
            this.Controls.Add(this.BodyPanel);
            this.Controls.Add(this.SettingsPanel);
            this.Font = new System.Drawing.Font("BIZ UDゴシック", 12F);
            this.Name = "Form1";
            this.Text = "Form1";
            this.SettingsPanel.ResumeLayout(false);
            this.SettingsPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.LineCellCount)).EndInit();
            this.BodyPanel.ResumeLayout(false);
            this.CompareResultPanel.ResumeLayout(false);
            this.CellsSettingListOwnerPanel.ResumeLayout(false);
            this.CellsSettingListOwnerPanel.PerformLayout();
            this.CompareSummaryPanel.ResumeLayout(false);
            this.CompareSummaryPanel.PerformLayout();
            this.CheckTargetPanel.ResumeLayout(false);
            this.CheckTargetPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel SettingsPanel;
        private System.Windows.Forms.Panel BodyPanel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button CompareButton;
        private System.Windows.Forms.TextBox FromFontNameText;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox ToFontSizeText;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox FromFontSizeText;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown LineCellCount;
        private System.Windows.Forms.CheckBox CheckWidth;
        private System.Windows.Forms.CheckBox CheckHeight;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckBox CheckFontSize;
        private System.Windows.Forms.CheckBox CheckFontName;
        private System.Windows.Forms.Panel CheckTargetPanel;
        private RectInfoInput RectInfoInput3;
        private RectInfoInput RectInfoInput2;
        private RectInfoInput RectInfoInput1;
        private RectInfoInput RectInfoInput0;
        private System.Windows.Forms.Panel CompareResultPanel;
        private System.Windows.Forms.Panel CompareSummaryPanel;
        private System.Windows.Forms.TextBox CompareSummaryText;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Panel CellsSettingListOwnerPanel;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Panel CellsSettingListPanel;
        private System.Windows.Forms.CheckBox CheckBorderDisappears;
        private System.Windows.Forms.Label label8;
    }
}

