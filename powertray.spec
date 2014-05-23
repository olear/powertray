Summary: Manage power states
Name: powertray

Version: 20140523.2
Release: 1%{dist}
License: GPLv2

Group: System Environment/Base
URL: http://www.dracolinux.org

Packager: Ole Andre Rodlie, <olear@dracolinux.org>
Vendor: DracoLinux, http://dracolinux.org

Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires: qt-devel
Requires: qt

%description
Manage power states.

%prep
%setup

%build
qmake-qt4 CONFIG-=debug CONFIG+=release PREFIX=/usr
make
strip -s build/%{name}

%install
make INSTALL_ROOT=%{buildroot} install

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_docdir}/%{name}-%{version}/*

%changelog
* Thu Apr 29 2014 Ole Andre Rodlie <olear@dracolinux.org> - 0.1-1
- initial version
