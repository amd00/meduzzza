
/*
 *  Copyright (C) 2013 Andrey Dudakov
 *
 *  Authors: Andrey "amd00" Dudakov
 *
 *  This file is part of meduzzza-gui.
 *
 *  meduzzza-gui is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  meduzzza-gui is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with meduzzza-gui.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SCANRESDIALOG_H_
#define _SCANRESDIALOG_H_

#include <QDialog>

namespace Ui
{
	class ScanResDialog;
}

namespace Meduzzza
{
	class ScanResDialog : public QDialog
	{
		Q_OBJECT
		
	private:
		Ui::ScanResDialog *m_ui;
		
	public:
		ScanResDialog(bool _is_mem);
		~ScanResDialog();
	};
}
#endif