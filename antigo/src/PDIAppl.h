

// Classe que define a aplicação
//================================

class TBmpViewApp : public TApplication {
  public:

	 TBmpViewApp(const char far* name) : TApplication(name) {}
	 void InitMainWindow();
  private:
	 TBmpViewWindow* client;
	 TControlBar* toolbar;
	 TStatusBar* statusLine;

};

void
 TBmpViewApp::InitMainWindow() {

		EnableCtl3d(TRUE);
		EnableBWCC();

		client = new TBmpViewWindow;

		TDecoratedMDIFrame * frame = new TDecoratedMDIFrame(
				"ProImagem",
				Name,
				*client,
				TRUE);
		frame->Attr.Style |= WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN |
		WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU |
		WS_THICKFRAME | WS_VISIBLE;
		frame->Attr.Style &= ~(WS_CHILD);

		nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWMAXIMIZED : nCmdShow;

//      client->SetFrame(frame);

		toolbar = new TControlBar(frame);

		toolbar->Insert(
			*new TButtonGadget(CM_CREATECHILD, CM_CREATECHILD));
		toolbar->Insert(
			*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
		toolbar->Insert(
			*new TButtonGadget(CM_FILESAVE, CM_FILESAVE));
		toolbar->Insert(
			*new TButtonGadget(CM_CASCADECHILDREN, CM_CASCADECHILDREN));
		toolbar->Insert(
			*new TButtonGadget(CM_TILECHILDREN, CM_TILECHILDREN));


		toolbar->SetHintMode(TGadgetWindow::EnterHints);

		frame->Insert(*toolbar, TDecoratedFrame::Top);

		statusLine = new TStatusBar(frame, TGadget::Recessed,
			TStatusBar::CapsLock | TStatusBar::NumLock | TStatusBar::ScrollLock |
			TStatusBar::Overtype);

		frame->Insert(*statusLine, TDecoratedFrame::Bottom);

		MainWindow = frame;
		MainWindow->AssignMenu(Name);
		MainWindow->SetIcon(this, IDI_APL);
	 }

