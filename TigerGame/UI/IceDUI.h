#ifndef ___ICE_DUI_H__
#define ___ICE_DUI_H__

#include "..\stdafx.h"

#define						WINDOW_MESSAGE					(WM_USER + 5100)
#define						MSG_CLOSE						1

//////////////////////////////////////////////////////////
//Base
#include "Base/ControlBase.h"
#include "Base/GlobalFunction.h"
#include "Base/MessageInterface.h"

//////////////////////////////////////////////////////////
//Timer
#include "Timer/Timer.h"
#include "Timer/Stopwatch.h"

//////////////////////////////////////////////////////////
//Button
#include "Button/ButtonEx.h"
#include "Button/CheckButton.h"
#include "Button/HideButton.h"
#include "Button/ImageButton.h"
#include "Button/LinkButton.h"
#include "Button/TextButton.h"

//////////////////////////////////////////////////////////
//List
#include "List/ListEx.h"
#include "List/ListGrid.h"

//////////////////////////////////////////////////////////
//Other
#include "Other/Area.h"
#include "Other/Frame.h"
#include "Other/ImageString.h"
#include "Other/Line.h"
#include "Other/ImageAnimation.h"
#include "Other/Scroll.h"
#include "Other/Slider.h"
#include "Other/SelectBox.h"

//////////////////////////////////////////////////////////
//Picture
#include "Picture/Picture.h"

//////////////////////////////////////////////////////////
//Progress
#include "Progress/Progress.h"

//////////////////////////////////////////////////////////
//Tab
#include "Tab/Tab.h"

//////////////////////////////////////////////////////////
//Text
#include "Text/StaticText.h"

//////////////////////////////////////////////////////////
//Edit
#include "Edit/EditEx.h"

//////////////////////////////////////////////////////////
//Dialog
#include "Dialog\DlgBase.h"
#include "Dialog\DlgPopup.h"
#include "Dialog\DlgBubble.h"

//////////////////////////////////////////////////////////
//Menu
#include ".\Menu\MenuItem.h"
#include ".\Menu\MenuEx.h"

#endif