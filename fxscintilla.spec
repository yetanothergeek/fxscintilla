%define name    fxscintilla
%define version 1.58.0
%define release 1

Summary: FXScintilla - Scintilla Editor Widget for FOX
Name:           %{name}       
Version:        %{version}
Release:        %{release}
License:	LGPL
Group: 		System Environment/Libraries
Source: 	fxscintilla-%{version}.tgz
Patch1: 	%{name}-test.patch
URL: 		http://savannah.gnu.org/projects/fxscintilla/
BuildRequires:	libfox0-devel >= 1.0
Requires: 	fox >= 1.0
Requires:	libfox0 >= 1.0
Requires: 	rpm >= 3.04
Packager: 	Laurent Julliard (laurent AT moldus DOT org)
Prefix: 	%{_prefix}
BuildRoot: 	%{_tmppath}/%{name}-%{version}-%{release}-buildroot

%description
FXScintilla is an implementation of the Scintilla Widget Editor for the FOX
Graphical User Interface toolkit. This package includes the library itself.

%package devel
Summary: Files for developping programs that use the Scintilla widget editor
Group: Development/Graphics
Requires: %{name} = %{PACKAGE_VERSION}
Requires:	libfox0-devel >= 1.0

%description devel
FXScintilla is an implementation of the Scintilla Widget Editor for
the FOX Graphical User Interface toolkit. This package includes the
library itself and all include files needed by developers.



%prep
%setup -q
%patch1 -p0 -b .test

%build
%configure --enable-nolexer
%make

%install
%makeinstall_std

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%doc COPYING INSTALL License.txt README
%{_libdir}/lib*.so.*


%files devel
%defattr(-,root,root)
%doc scintilla/doc
%{_includedir}/fxscintilla/SciLexer.h
%{_includedir}/fxscintilla/Scintilla.h
%{_includedir}/fxscintilla/FXScintilla.h
%{_libdir}/lib*.la
%{_libdir}/lib*.a

%changelog
* Mon Jan 26 2004 Gilles Filippini (pini AT free DOT fr) 1.58-1
- upgraded to 1.58

* Mon Jan 05 2004 Gilles Filippini (pini AT free DOT fr) 1.57-1
- upgraded to 1.57

* Sun Sep 28 2003 Gilles Filippini (pini AT free DOT fr) 1.55-1
- upgraded to 1.55

* Thu Aug 07 2003 Gilles Filippini (pini AT free DOT fr) 1.53-1
- upgraded to 1.53

* Sat Apr 19 2003 Gilles Filippini (pini AT free DOT fr) 1.52-1
- upgraded to 1.52

* Sun Feb 16 2003 Gilles Filippini (pini AT free DOT fr) 1.51-1
- upgraded to 1.51

* Sun Nov 03 2002 Gilles Filippini (pini AT free DOT fr) 1.49-1
- upgraded to 1.49

* Wed May 29 2002 Laurent Julliard (laurent AT moldus DOT org) 1.46-1
- upgraded to 1.46.1
- enabled the generation of the nolexer library (--enable-nolexer)
- made a patch to test.cpp for a correct include of fx.h

* Wed May 29 2002 Laurent Julliard (laurent AT moldus DOT org) 1.46-1
- initial package

* Wed May 29 2002 Laurent Julliard (laurent AT moldus DOT org) 1.46-1
- initial package
