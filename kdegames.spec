%define version Beta3
%define name kdegames
Name: %{name}
Summary: K Desktop Environment - Games
Version: %{version}
Release: 1
Source: ftp.kde.org:/pub/unstable/CVS/snapshots//current/%{name}-%{version}.tar.gz
Group: X11/KDE/Games
BuildRoot: /tmp/realhot_%{name}
Copyright: GPL
Requires: qt >= 1.31
Packager: Magnus Pfeffer <pfeffer@unix-ag.uni-kl.de>
Distribution: KDE
Vendor: The KDE Team

%description

Games for the K Desktop Environment.

Included with this package are:

kabalone: a strategy game 
kasteroids: arcade game
kmahjongg: the popular mahjongg game
kmines: mine sweeper
kpat: several card games, including solitaire
kpoker: video poker game
kreversi: the Reversi game
ksame: board game
kshisen: Shisen-Sho - a game related to mahjongg
ksnake: snake race
ktetris: the well-known tetris game

%prep
rm -rf $RPM_BUILD_ROOT

%setup -n kdegames

%build
export KDEDIR=/opt/kde
./configure --prefix=$KDEDIR --with-install-root=$RPM_BUILD_ROOT
cd kasteroids
moc -o view.moc view.h
moc -o toplevel.moc toplevel.h
moc -o kfixedtopwidget.moc kfixedtopwidget.h

cd ..
make 

%install
make install

cd $RPM_BUILD_ROOT
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > $RPM_BUILD_DIR/file.list.%{name}
find . -type f | sed 's,^\.,\%attr(-\,root\,root) ,' >> $RPM_BUILD_DIR/file.list.%{name}
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >> $RPM_BUILD_DIR/file.list.%{name}

%clean
rm -rf $RPM_BUILD_ROOT

%files -f ../file.list.%{name}
