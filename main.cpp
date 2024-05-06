#include "main.h"
#include "rc.h"
#include <filesystem>

void main_window::on_paint(HDC hdc) 
{
	RECT rt;
	GetClientRect(*this, &rt);

	if (lImage) {
		Gdiplus::Graphics gr(hdc);
		Gdiplus::RectF displayRect(rt.left, rt.top, rt.right, rt.bottom);
		gr.DrawImage(lImage.get(), displayRect);

		Gdiplus::StringFormat t;
		t.SetAlignment(Gdiplus::StringAlignmentCenter);
		t.SetLineAlignment(Gdiplus::StringAlignmentFar);
		Gdiplus::Font font(_T("Arial"), 25, Gdiplus::FontStyleBold);
		Gdiplus::SolidBrush tForm(Gdiplus::Color::Black);
		gr.DrawString(fileName.c_str(), -1, &font, displayRect, &t, &tForm);
		

		displayRect.Width = rt.right - 5;
		displayRect.Height = rt.bottom - 5;
		tForm.SetColor(Gdiplus::Color::White);
		gr.DrawString(fileName.c_str(), -1, &font, displayRect, &t, &tForm);
	}
}

void main_window::on_command(int id) 
{
	switch (id)
	{
	case ID_OPEN:
	{
		TCHAR path[MAX_PATH] = _T("");
		OPENFILENAME ofn{ sizeof OPENFILENAME };

		ofn.hwndOwner = *this;
		ofn.lpstrFile = path;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = _T("Image Files (*.jpg;*.png;*.bmp; *.gif;*.tiff;*.emf)\0*.jpg;*.png;*.bmp; *.gif;*.tiff;*.emf\0All Files (*.*)\0*.*\0");
		ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

		if (::GetOpenFileName(&ofn)) {
			lImage = std::make_unique<Gdiplus::Image>(path);
			fileName = std::filesystem::path(path).filename();
			::InvalidateRect(*this, nullptr, TRUE);
		}
		break;
	}
	case ID_EXIT:
		DestroyWindow(*this);
		break;
	}
}

void main_window::on_destroy() 
{
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
