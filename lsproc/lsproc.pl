#! c:\perl\bin\perl.exe
#-----------------------------------------------------------------
# lsproc.pl - parse Windows 2000 phys. memory/RAM dump,
#             looking for processes.
#
# copyright 2006 H. Carvey, keydet89@yahoo.com
#-----------------------------------------------------------------
use strict;

my $file = shift || die "You must enter a filename.\n";
die "$file not found.\n" unless (-e $file);

my $record;

open(FH,"<",$file) || die "Could not open $file : $!\n";
binmode(FH);

my $offset = 0;

printf "%-4s  %-6s %-6s  %-20s %-10s  %-10s %-10s\n","Type","PPID","PID","Name","Offset","FLink","BLink";
printf "%-4s  %-6s %-6s  %-20s %-10s  %-10s %-10s\n","-" x 4,"-" x 4,"-" x 3,"-" x 4,"-" x 6,"-" x 5,"-" x 5;
while (! eof(FH)) {	
	seek(FH,$offset,0);
	read(FH,$record,4);
	my $hdr = unpack("V",$record);
	if ($hdr == 0x001b0003) {
# Possible EPROCESS block located, let's run some checks
#		printf "Possible EPROCESS block located at offset 0x%08x\n",$offset;
		my $data;
		seek(FH,$offset,0);
		my $bytes = read(FH,$data,0x290);
		if (0x290 == $bytes) {
			if (my %proc = isProcess($data)) {
				my $name;
				($proc{exitprocesscalled} == 1) ? ($name = $proc{name}."(x)") : ($name = $proc{name});
				printf "%-4s  %-6d %-6d  %-20s 0x%08x  0x%08x 0x%08x\n","Proc",$proc{ppid},$proc{pid},$name,$offset,
						$proc{flink},$proc{blink};
				$offset += 0x290;
			}
			else {
				$offset += 4;
			}
		}
		else {
#			print "Too few bytes read.\n";
#			exit 1;
		}
	}
	elsif ($hdr == 0x006c0006) {
# Possible ETHREAD found
		my $data;
		seek(FH,$offset,0);
		my $bytes = read(FH,$data,0x244);
		if ($bytes == 0x244) {
			if (my %thread = isThread($data)) {
				
				$offset += 0x244;
			}
			else {
				$offset += 4;
			}
		}
	}
	else {
# Increment the offset count by 4 bytes, or one DWORD		
		$offset += 4;		
	}
}
close(FH);


#---------------------------------------------------------
# isProcess()
# check to see if we have a valid process (Win2K SP4)
# Input : 652 bytes starting at the offset
# Output: Hash containing EPROCESS block info, undef if not a valid 
#         EPROCESS block
#---------------------------------------------------------
sub isProcess {
	my $data = shift;
	my %proc = ();
	my $event1 = unpack("V",substr($data,0x13c,4));
	my $event2 = unpack("V",substr($data,0x164,4));
	
	if ($event1 == 0x40001 && $event2 == 0x40001) {
# Use this area to populate the EPROCESS structure		
		my $name = substr($data,0x1fc,16);
		$name =~ s/\00//g;
		$proc{name} = $name;
#		$proc{exitstatus} = unpack("V", substr($data,0x06c,4));
# Get Active Process Links for EPROCESS block
		($proc{flink},$proc{blink})  = unpack("VV",substr($data,0x0a0,8));
#		my (@createTime) 	= unpack("VV", substr($data,0x088,8));
#		$proc{createtime} = getTime($createTime[0],$createTime[1]);
#		my (@exitTime) = unpack("VV", substr($data,0x090,8));
#		$proc{exittime} = getTime($exitTime[0],$exitTime[1]);
		
#		$proc{pObjTable} = unpack("V",substr($data,0x128,4));
#		$proc{pSectionHandle} = unpack("V",substr($data,0x1ac,4));
#		$proc{pSecBaseAddr} = unpack("V",substr($data,0x1b4,4));
		
		$proc{pid} = unpack("V",substr($data,0x09c,4));
	  $proc{ppid}	= unpack("V",substr($data,0x1c8,4));
#	  ($proc{subsysmin},$proc{subsysmaj}) = unpack("CC",substr($data,0x212,2));
#		$proc{directorytablebase} = unpack("V",substr($data,0x018,4));
#	  $proc{peb} = unpack("V",substr($data,0x1b0,4));
	  $proc{exitprocesscalled} = unpack("C",substr($data,0x1aa,1));
	  $proc{pimagefilename} = unpack("V",substr($data,0x284,4));
	}
	else {
# Not an EPROCESS block
	}
	return %proc;
}

#---------------------------------------------------------
# isThread()
# check to see if we have a valid thread (Win2K SP4)
# Input : 0x244 bytes starting at the offset
# Output: Hash containing ETHREAD block info, undef if not a valid 
#         ETHREAD block
#---------------------------------------------------------
sub isThread {
	my $data = shift;
	my %thread = ();
#	my $ktimer = unpack("V",substr($data,0x0e8,4));
	my $sync1 = unpack("V",substr($data,0x190,4));
	my $sync2 = unpack("V",substr($data,0x1e8,4));
	
	if ($sync1 == 0x50005 && $sync2 == 0x50005) {
		($thread{pid},$thread{tid}) = unpack("VV",substr($data,0x1e0,8));
		$thread{hasterminated} = unpack("V",substr($data,0x224,4));
		my (@createTime) 	= unpack("VV", substr($data,0x1b0,8));
		$thread{createtime} = getTime($createTime[0],$createTime[1]);
		my (@exitTime) = unpack("VV", substr($data,0x1b8,8));
		$thread{exittime} = getTime($exitTime[0],$exitTime[1]);
		$thread{hidefromdebugger} = unpack("C",substr($data,0x223,1));
	}
	return %thread;
}

#---------------------------------------------------------
# getPDE()
# Get Page Directory Entry
# Input : physical file offset
# Output: DWORD located at that offset
#---------------------------------------------------------
sub getPDE {
	my $pda = shift;
	my $record;
	seek(FH,$pda,0);
	read(FH,$record,4);	
	return unpack("V",$record);
}

#---------------------------------------------------------
# getPTE()
# Get Page Table Entry
# Input : physical file offset
# Output: DWORD located at that offset
#---------------------------------------------------------
sub getPTE {
	my $pta = shift;
	my $record;
	seek(FH,$pta,0);
	read(FH,$record,4);	
	return unpack("V",$record);
}

#---------------------------------------------------------
# getOffset()
# Get physical offset within dump, based on logical addresses
# Translates a logical address to a physical offset w/in the dump
#   file
# Input : two addresses (ex: PEB and DirectoryTableBase)
# Output: offset within file
#---------------------------------------------------------
sub getOffset {
	my $peb = shift;
	my $dtb = shift;
	my $pdi = $peb >> 22 & 0x3ff;
	my $pda = $dtb + ($pdi * 4);
	my $pde = getPDE($pda);
# Determine page size if needed
# $pde & 0x080; if 1, page is 4Mb; else, 4Kb
# Check to see if page is present
	if ($pde & 0x1) {
		my $pti = $peb >> 12 & 0x3ff;
		my $ptb = $pde >> 12;
		my $pte = getPTE(($ptb * 0x1000) + ($pti * 4));
		if ($pte & 0x1) {
			my $pg_ofs = $peb & 0x0fff;
			return ((($pte >> 12) * 0x1000) + $pg_ofs);	
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

#---------------------------------------------------------
# getTime()
# Get Unix-style date/time from FILETIME object
# Input : 8 byte FILETIME object
# Output: Unix-style date/time
#---------------------------------------------------------
sub getTime() {
	my $lo = shift;
	my $hi = shift;
	my $t;

	if ($lo == 0 && $hi == 0) {
		$t = 0;
	} else {
		$lo -= 0xd53e8000;
		$hi -= 0x019db1de;
		$t = int($hi*429.4967296 + $lo/1e7);
	};
	$t = 0 if ($t < 0);
	return $t;
}

#----------------------------------------------------------------
# _uniToAscii()
# Input : Unicode string
# Output: ASCII string
# Removes every other \00 from Unicode strings, returns ASCII string
#----------------------------------------------------------------
sub _uniToAscii {
	my $str = $_[0];
	my $len = length($str);
	my $newlen = $len - 1;
	my @str2;
	my @str1 = split(//,$str,$len);
	foreach my $i (0..($len - 1)) {
		if ($i % 2) {
# In a Unicode string, the odd-numbered elements of the list will be \00
# so just drop them			
		}
		else {
			push(@str2,$str1[$i]);
		}
	}
	return join('',@str2);
}