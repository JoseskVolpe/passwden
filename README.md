# passwden
Simple CLI password manager written in C with GnuPG back-end. Alpha stage.

## Features
* Use in command-line
* Written in C
* Cryptography using GnuPG
* Organize your passwords to websites
* Easy to change your key. One single command and your passwords are reencrypted with a new key
* Lightweight

## Dependencies
Check your distribution's package manager.

These packages are required to use this tool:
* GPGME
* json-c
* libinih
* libc (Alternative C assemblers may work)

These packages are required to build this tool:
* gcc (Alternative compilers like musl-gcc may work, but it wasn't tested)
* make
* pkgconf

## Build
### Compile
`make`
### Install
`[DESTDIR=PATH/TO/EXTRACT] make install`

Note: Check your distribution's packaging manual to install system-wide

## Usage
```passwden {WEBSITE [LOGIN]}: Show passwords

passwden OPTION
Options:
{--set | -s} Add or update a account password
{--remove | -r} <WEBSITE> [LOGIN] : Remove a website or password
{--help | -h}: Show help
{--key | -k} [FINGERPRINT]: Show key fingerprint / set key fingerprint. Passwords will be reencrypt after updating your fingerprint
{--list | -l} [WEBSITE]: List registered websites and login
{--version | -v}: Show version


To use this program, you need to first apply your fingerprint.
It's STRONGELY RECOMMEND that you use a key exclusively for this purpose and that you secure the private key with a password.
To search for your keys fingerprints, use
gpg --list-secret-keys --keyid-format=long [NAME OR EMAIL]

Avoid using this program in a default terminal as the password will be printed. Execute, then clear (or exit)
```


## License
 Copyright (C) 2022  Josesk Volpe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
