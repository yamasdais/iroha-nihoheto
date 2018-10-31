using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Reactive.Disposables;
using System.Reactive.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Markup;
using System.Collections;

namespace RegexSensei
{
    class RegexSenseiViewModel : IDisposable, INotifyPropertyChanged {
        private RegexSenseiModel model = new RegexSenseiModel();
        private CompositeDisposable disposable { get; } = new CompositeDisposable();

        public ReactiveProperty<string> InputPattern { get; set; }

        public ReactiveProperty<string> StatusMessage { get; set; }

        public ReactiveProperty<bool> IgnoreCase { get; set; }
        public ReactiveProperty<bool> Multiline { get; set; }
        public ReactiveProperty<bool> ExplicitCapture { get; set; }
        public ReactiveProperty<bool> Compiled { get; set; }
        public ReactiveProperty<bool> Singleline { get; set; }
        public ReactiveProperty<bool> IgnorePatternWhitespace { get; set; }
        public ReactiveProperty<bool> RightToLeft { get; set; }
        public ReactiveProperty<bool> ECMAScript { get; set; }
        public ReactiveProperty<bool> CultureInvariant { get; set; }
        public ReactiveProperty<string> Text { get; set; }

        public string OutputMessage { get; set; } = "";
        //public FlowDocument OutputDocument { get; set; }
        public FlowDocument PremadeDocument { get; set; }
        public FlowDocument OutputDocument { get; set; }

        public ReactiveCommand RunMatchRegex { get; }
            
        public event PropertyChangedEventHandler PropertyChanged;

        public RegexSenseiViewModel() {
            InputPattern = new ReactiveProperty<string>("").AddTo(this.disposable);
            StatusMessage = new ReactiveProperty<string>("").AddTo(this.disposable);
            Multiline = new ReactiveProperty<bool>(false).AddTo(this.disposable);
            IgnoreCase = new ReactiveProperty<bool>(false).AddTo(this.disposable);
            ExplicitCapture = new ReactiveProperty<bool>(false).AddTo(this.disposable);
            Compiled = new ReactiveProperty<bool>(false).AddTo(this.disposable);
            Singleline = new ReactiveProperty<bool>(false).AddTo(this.disposable);
            IgnorePatternWhitespace = new ReactiveProperty<bool>(false).AddTo(this.disposable);
            RightToLeft = new ReactiveProperty<bool>(false).AddTo(this.disposable);
            ECMAScript = new ReactiveProperty<bool>(false).AddTo(this.disposable);
            CultureInvariant = new ReactiveProperty<bool>(false).AddTo(this.disposable);
            Text = new ReactiveProperty<string>("").AddTo(this.disposable);
            PremadeDocument = new FlowDocument();
            OutputDocument = new FlowDocument();

            RunMatchRegex = new ReactiveCommand().AddTo(disposable);
            //OutputDocument = new FlowDocument();
            //OutputDocument.Blocks.Add(new Paragraph(new Run("foo bar baz")));


            InputPattern.Delay(TimeSpan.FromMilliseconds(500)).Subscribe(_ => {
                try {
                    model.Pattern = InputPattern.Value;
                    StatusMessage.Value = "OK";
                    //MatchCommand();
                } catch (Exception e) {
                    StatusMessage.Value = e.Message;
                }
            });

            Text.Delay(TimeSpan.FromMilliseconds(500)).Subscribe(_ => {
                try {
                    model.Text = Text.Value;
                    //MatchCommand();
                } catch (Exception e) {
                    StatusMessage.Value = e.Message;
                }
            });

            RunMatchRegex.Subscribe(_ => {
                MatchCommand();
            });

            IgnoreCase.Subscribe(_ => model.SetOption(RegexOptions.IgnoreCase, IgnoreCase.Value));
            Multiline.Subscribe(_ => model.SetOption(RegexOptions.Multiline, Multiline.Value));
            ExplicitCapture.Subscribe(_ => model.SetOption(RegexOptions.ExplicitCapture, ExplicitCapture.Value));
            Compiled.Subscribe(_ => model.SetOption(RegexOptions.Compiled, Compiled.Value));
            Singleline.Subscribe(_ => model.SetOption(RegexOptions.Singleline, Singleline.Value));
            IgnorePatternWhitespace.Subscribe(_ => model.SetOption(RegexOptions.IgnorePatternWhitespace, IgnorePatternWhitespace.Value));
            RightToLeft.Subscribe(_ => model.SetOption(RegexOptions.RightToLeft, RightToLeft.Value));
            ECMAScript.Subscribe(_ => model.SetOption(RegexOptions.ECMAScript, ECMAScript.Value));
            CultureInvariant.Subscribe(_ => model.SetOption(RegexOptions.CultureInvariant, CultureInvariant.Value));
        }

        public void Dispose() => disposable.Dispose();

        protected virtual void OnPropertyChanged(string name) {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

#if false
        private void ProcessMatchedGroup(FlowDocument doc, int idx, Group g) {
        Console.WriteLine($"${idx} {g.Index},{g.Length}: ");
            doc.Blocks.Add(new Paragraph(new Run($"${idx} {g.Index},{g.Length}: ")));
            var range = new TextRange(doc.ContentStart, doc.ContentEnd);
            range.Text += $"${idx} {g.Index},{g.Length}: ";
        }
#endif
        private void ProcessMatchedGroup(int idx, Group g) {
        Console.WriteLine($"${idx} {g.Index},{g.Length}: ");
            OutputMessage += $"${idx} {g.Index},{g.Length}: ";
        }

        private void MatchCommand() {
            var matches = model.Match();
            if (matches != null) {
                OutputMessage = "";
                foreach (Match match in matches) {
                    var idx = 0;
                    foreach (Group group in match.Groups) {
                        ProcessMatchedGroup(idx, group);
                        idx++;
                    }
                }
                OnPropertyChanged(nameof(OutputMessage));
                // OutputDocument= updated_document;
                // OnPropertyChanged("OutputDocument");
                //this.OutputDocument = PremadeDocument;
            }
        }
    }

    public class BindableRichTextBox : RichTextBox {
        public static readonly DependencyProperty DocumentProperty =
            DependencyProperty.Register(
                "Document",
                typeof(FlowDocument),
                typeof(BindableRichTextBox),
                new UIPropertyMetadata(null, OnTextItemChanged));

        public new FlowDocument Document {
            get {
                return (FlowDocument)GetValue(DocumentProperty);
            }
            set {
                SetValue(DocumentProperty, value);
            }
        }

        private static void OnTextItemChanged(DependencyObject sender, DependencyPropertyChangedEventArgs ev) {
            var control = sender as RichTextBox;
            if (control != null) {
                control.Document = ev.NewValue as FlowDocument;
            }
        }
    }

    public class RichTextBoxHelper : DependencyObject {
        private static HashSet<Thread> _recursionProtection = new HashSet<Thread>();

        public static string GetDocumentXaml(DependencyObject obj) {
            return (string)obj.GetValue(DocumentXamlProperty);
        }

        public static void SetDocumentXaml(DependencyObject obj, string value) {
            _recursionProtection.Add(Thread.CurrentThread);
            obj.SetValue(DocumentXamlProperty, value);
            _recursionProtection.Remove(Thread.CurrentThread);
        }

        public static readonly DependencyProperty DocumentXamlProperty =
            DependencyProperty.RegisterAttached(
                "DocumentXaml",
                typeof(string),
                typeof(RichTextBoxHelper),
        new FrameworkPropertyMetadata(
            "",
            FrameworkPropertyMetadataOptions.AffectsRender | FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
            (obj, e) => {
                if (_recursionProtection.Contains(Thread.CurrentThread))
                    return;

                var richTextBox = (RichTextBox)obj;

                // Parse the XAML to a document (or use XamlReader.Parse())

                try {
                    TextRange range = new TextRange(richTextBox.Document.ContentStart, richTextBox.Document.ContentEnd);
                    //var stream = new MemoryStream(Encoding.UTF8.GetBytes(GetDocumentXaml(richTextBox)));
                    var message = GetDocumentXaml(richTextBox);
                    var stream = new MemoryStream();
                    XamlWriter.Save(range, stream);
                    range.Save(stream, DataFormats.XamlPackage);
                    var output = new FlowDocument(new Paragraph(new Run(message)));
                    //TextRange out_range = new TextRange(output.ContentEnd, output.ContentEnd);
                    //out_range.Load(stream, DataFormats.XamlPackage);
                    //var doc = (FlowDocument)XamlReader.Load(stream);

                    // Set the document
                    richTextBox.Document = output;
                } catch (Exception ex) {
                    richTextBox.Document = new FlowDocument(new Paragraph(new Run(ex.Message)));
                }

                // When the document changes update the source
                richTextBox.TextChanged += (obj2, e2) => {
                    RichTextBox richTextBox2 = obj2 as RichTextBox;
                    if (richTextBox2 != null) {
                        SetDocumentXaml(richTextBox, XamlWriter.Save(richTextBox2.Document));
                    }
                };
            }
        )
    );
    }
}
