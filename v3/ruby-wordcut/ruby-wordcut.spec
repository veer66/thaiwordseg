Summary: Ruby/Wordcut is a Ruby interface to the Wordcut
Name: ruby-wordcut
Version: 0.1.2
Release: 1
License: GPL
Group: Applications/Ruby
Source: %{name}-%{version}.tar.gz
URL: http://thaiwordseg.sourceforge.net>
Packager: Vee Satayamas <virasj@users.sourceforge.net>
BuildRoot: /var/tmp/%{name}-%{version}
BuildRequires: ruby, wordcut-devel >= 0.1.2
Requires: ruby >= 1.6.7 , wordcut >= 0.1.2

%description
Ruby/Wordcut is a Ruby interface to the Wordcut.

%prep
%setup

%build
ruby extconf.rb
make

%clean
rm -rf $RPM_BUILD_ROOT

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
find $RPM_BUILD_ROOT -type f -print | \
        ruby -pe 'sub("^'$RPM_BUILD_ROOT'", "")' > %{name}-%{version}-filelist

%files -f %{name}-%{version}-filelist
%defattr(-,root,root)
# %doc -P doc/ sample/
%doc COPYING README AUTHORS VERSION ChangeLog

%changelog
