#! /bin/sh

# Generate scribo/headers.mk
# ------------------------------
# List all the headers in order to make them part of distribution.

# Use the C locale to have a deterministic sort.
export LC_ALL=C

echo "## Generated by ./generate_dist_headers, do not modify ##" > headers.mk
echo "" >> headers.mk
echo "noinst_HEADERS = \\" >> headers.mk
echo "generating headers.mk"

find . -type f -name '*.hh'| grep -v "\.svn" | sed -e 's/$/ \\/g' | sort >> headers.mk
find . -type f -name '*.hxx'| grep -v "\.svn" | sed -e 's/$/ \\/g'| sort >> headers.mk

last_line=`tail -n 1 headers.mk | sed -e 's/\\\//g'` # remove '\' in last line
sed '$d' < headers.mk > headers.mk.tmp # remove last line
mv headers.mk.tmp headers.mk
echo $last_line >> headers.mk # put the cleaned last line back.