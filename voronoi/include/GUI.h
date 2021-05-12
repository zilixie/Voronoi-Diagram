#ifndef GUI_H
#define GUI_H

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

class GUI {
	public:
		nanogui::Screen *screen;
		nanogui::Window *nanoguiWindow;
		nanogui::ComboBox *cobo;
		nanogui::ComboBox *cobo2;
		nanogui::ColorPicker *cp;

		bool focus_check(void);
		void nanogui_init(GLFWwindow* window, int x, int y);
};

inline bool GUI::focus_check() {
	if (nanoguiWindow->focused() ||
		cobo->focused() ||
		cobo->popup()->focused() ||
		cobo2->focused() ||
		cobo2->popup()->focused() ||
		cp->focused() ||
		cp->popup()->focused()) {
		return true;
	} else {
		return false;
	}
}

inline void GUI::nanogui_init(GLFWwindow* window, int x, int y) {
	screen = new nanogui::Screen();
	screen->initialize(window, true);
	nanoguiWindow = new nanogui::Window(screen, "Control Panel");
	nanoguiWindow->setPosition(Vector2i(x, y));
	nanoguiWindow->setLayout(new nanogui::GroupLayout());
	nanogui::Theme *t = new nanogui::Theme(screen->nvgContext());
	t->mWindowPopup = nanogui::Color(43, 230);
	t->mButtonFontSize = 16;
	t->mWindowCornerRadius = 0;
	t->mWindowDropShadowSize = 0;

	screen->setTheme(t);
	new nanogui::Label(nanoguiWindow, "Switch Mode :", "sans-bold");
	cobo = new nanogui::ComboBox(nanoguiWindow, { "Translate", "Scale", "Rotate", "Delete" });
	cobo->setFixedSize(Vector2i(100,20));
	cobo->setCallback([](int a) {
	});
	new nanogui::Label(nanoguiWindow, "Add Object :", "sans-bold");
	cobo2 = new nanogui::ComboBox(nanoguiWindow, {"_", "_", "_", "_" ,"_" ,"_" });
	cobo2->setFixedSize(Vector2i(100,20));
	cobo2->setCallback([this](int a) {
		this->cobo->setSelectedIndex(0);
	});
	new nanogui::Label(nanoguiWindow, "Change Color :", "sans-bold");
	cp = new nanogui::ColorPicker(nanoguiWindow, {191,51,46, 255});
	cp->setFixedSize({100, 20});
	// cp->setFinalCallback([e](const nanogui::Color &c) {
	// 	std::cout << "ColorPicker: ["<< c.r() << ", " << c.g() << ", " << c.b() << ", " << c.w() << "]" << std::endl;
	// 	e.cur_col = Eigen::Vector3f(c.r()*255.0, c.g()*255.0, c.b()*255.0);
	// });

	nanogui::GridLayout *layout = new nanogui::GridLayout(nanogui::Orientation::Horizontal, 2,nanogui::Alignment::Middle, 15, 5);
	layout->setColAlignment({nanogui::Alignment::Maximum, nanogui::Alignment::Fill});
	layout->setSpacing(0, 10);
	nanoguiWindow->setLayout(layout);

	screen->setVisible(true);
	screen->performLayout();
}

#endif