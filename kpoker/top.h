#include <ktopwidget.h>

class QPopupMenu;
class KMenuBar;
class KToolBar;
class KStatusBar;
class kpok;


class PokerWindow : public KTopLevelWidget
{
	Q_OBJECT;
private:
	KMenuBar        *menu;
	
	QPopupMenu      *filePopup;
	QPopupMenu      *helpPopup;
	
	kpok            *_kpok;
	
public:
	PokerWindow();
	~PokerWindow();
};
