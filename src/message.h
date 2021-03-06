  /*
 * message.h
 *
 * Copyright 2022 Josesk Volpe <joseskvolpe@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

 #define INVALID_FINGERPRINT_LENGTH_MESSAGE "Invalid fingerprint, value must have 16 characters\n"
 #define INVALID_FINGERPRINT_NOT_FOUND_MESSAGE "Invalid fingerprint, private key not found\n"
 #define FINGERPRINT_NOT_DEFINED "You must choose your private key fingerprint first. Use: \n%s --key <FINGERPRINT>\nWith GnuPG, you may find your fingerprints by using:\ngpg --list-secret-keys --keyid-format=long\n"
 #define GPGME_VERSION_OUTDATED "gpgme required version is %s, you got %s. Please, update it\n"
 #define GPGME_ERROR "gpgme error %s, exiting...\n"
 #define GPGME_CIPHERTEXT_ERROR "Error reading ciphertext\nMax buffer is %lu, got %lu\n"
 #define ERROR_REENCRYPTING_FILE_MESSAGE "Couldn't reencrypt passwords, fingerprint apply cancelled.\nPlease, check you still have your old private key\n"
