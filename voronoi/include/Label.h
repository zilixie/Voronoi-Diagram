#ifndef LABEL_H
#define LABEL_H

#include <nanogui/nanogui.h>
#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <nanogui/theme.h>
#include "Editor.h"

class Label {
	public:
		nanogui::Screen *screen;
		nanogui::Window *nanoguiWindow;

		void label_init(GLFWwindow* window, int x, int y, std::string text, Eigen::Vector4i c);
		void label_init(GLFWwindow* window, int x, int y, std::string text);
};

inline void Label::label_init(GLFWwindow* window, int x, int y, std::string text, Eigen::Vector4i c) {
	screen = new nanogui::Screen();
	screen->initialize(window, true);
	nanoguiWindow = new nanogui::Window(screen, text);
	nanoguiWindow->setPosition(Vector2i(x, y));
	nanoguiWindow->setLayout(new nanogui::GroupLayout());
	nanogui::Theme *t = new nanogui::Theme(screen->nvgContext());

	t->mWindowCornerRadius = 0;
	t->mWindowDropShadowSize = 0;
	// nanogui::Color(43, 230);
	t->mWindowHeaderGradientTop = nanogui::Color(c(0), c(1), c(2), c(3));
	t->mWindowHeaderGradientBot = nanogui::Color(c(0), c(1), c(2), c(3));

	t->mWindowFillUnfocused = nanogui::Color(255, 255, 255, 0);
	t->mWindowFillFocused = nanogui::Color(255, 255, 255, 0);

	t->mTransparent = nanogui::Color(255, 255, 255, 0);

	t->mWindowHeaderSepTop = nanogui::Color(255, 255, 255, 0);
	t->mWindowHeaderSepBot = nanogui::Color(255, 255, 255, 0);

	// text color
	t->mWindowTitleUnfocused = nanogui::Color(0, 255);
	t->mWindowTitleFocused = nanogui::Color(0, 255);

	screen->setTheme(t);

	screen->setVisible(true);
	screen->performLayout();
}

inline void Label::label_init(GLFWwindow* window, int x, int y, std::string text) {
	screen = new nanogui::Screen();
	screen->initialize(window, true);
	nanoguiWindow = new nanogui::Window(screen, text);
	nanoguiWindow->setPosition(Vector2i(x, y));
	nanoguiWindow->setLayout(new nanogui::GroupLayout());
	nanogui::Theme *t = new nanogui::Theme(screen->nvgContext());

	t->mWindowCornerRadius = 0;
	t->mWindowDropShadowSize = 0;
	// nanogui::Color(43, 230);
	t->mWindowHeaderGradientTop = nanogui::Color(255, 255, 255, 0);
	t->mWindowHeaderGradientBot = nanogui::Color(255, 255, 255, 0);

	t->mWindowFillUnfocused = nanogui::Color(255, 255, 255, 0);
	t->mWindowFillFocused = nanogui::Color(255, 255, 255, 0);

	t->mTransparent = nanogui::Color(255, 255, 255, 0);

	t->mWindowHeaderSepTop = nanogui::Color(255, 255, 255, 0);
	t->mWindowHeaderSepBot = nanogui::Color(255, 255, 255, 0);

	// text color
	t->mWindowTitleUnfocused = nanogui::Color(0, 255);
	t->mWindowTitleFocused = nanogui::Color(0, 255);

	screen->setTheme(t);

	screen->setVisible(true);
	screen->performLayout();
}

#endif