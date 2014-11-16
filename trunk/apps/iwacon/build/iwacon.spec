%{!?python_site: %define python_site %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(0)")}

Name:           iwacon
Version:        0.0.1
Release:        1%{?dist}
Summary:        iWattnick configuration tool
Group:          Applications/Engineering
License:        GPLv3+
URL:            https://code.google.com/p/iwattnick/
Source0:        iwacon-%{version}.tar.bz2
BuildArch:      noarch
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:  python
BuildRequires:  desktop-file-utils
Requires:       python pygtk2 pyserial
Requires:       desktop-file-utils

%description
This is the program to configure and debug the iWattnick
on-board computer for e-bikes, e-scooters and e-motorcycles.

%prep
%setup -q

%build
# nothing to do :)

%install
rm -rf $RPM_BUILD_ROOT
%{__python} setup.py install -O1 --root $RPM_BUILD_ROOT
%find_lang %{name}

%clean
rm -rf $RPM_BUILD_ROOT

%files -f %{name}.lang
%defattr(-,root,root,-)
%doc debian/changelog
%{python_site}/iwattnick/*
%{python_site}/iwattnick-%{version}*-info
%{_bindir}/iwacon
%{_bindir}/iwacon-cli
%{_datadir}/applications/*.desktop
%{_datadir}/pixmaps/*
%{_datadir}/%{name}/*

%post
update-desktop-database -q

%postun
update-desktop-database -q

%changelog
* Sat Oct 18 2014 Andrey Zabolotnyi <andrey.zabolotnyi@ensoltech.ru> - 0.2.1-1
- Minor fixes for Windows
