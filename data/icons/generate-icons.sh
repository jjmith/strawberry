#!/bin/sh

sizes="128x128 64x64 48x48 32x32 22x22"

# 

for i in full/*
do
  source=$i
  file=`basename $i`

  id=`identify "$i"` || exit 1
  if [ "$id" = "" ] ; then
    echo "ERROR: Cannot dermine format and geometry for image: \"$i\"."
    continue
  fi
  g=`echo $id | awk '{print $3}'` || exit 1
  if [ "$g" = "" ] ; then
    echo "ERROR: Cannot dermine geometry for image: \"$i\"."
    continue
  fi
  
  # Geometry can be 563x144+0+0 or 75x98
  # we need to get rid of the plus (+) and the x characters:
  w=`echo $g | sed 's/[^0-9]/ /g' | awk '{print $1}'` || exit 1
  if [ "$w" = "" ] ; then
    echo "ERROR: Cannot dermine width for image: \"$x\"."
    continue
  fi
  h=`echo $g | sed 's/[^0-9]/ /g' | awk '{print $2}'` || exit 1
  if [ "$h" = "" ] ; then
    echo "ERROR: Cannot dermine height for image: \"$x\"."
    continue
  fi
  
  for x in $sizes
  do
  
    dest="$x/$file"
    if [ -f $dest ]; then
      continue
    fi

    x_w=$(echo $x | cut -d 'x' -f1)
    x_h=$(echo $x | cut -d 'x' -f2)

    if [ "$w" -lt "$x_w" ] || [ "$h" -lt "$x_h" ]; then
      continue
    fi

    echo "convert -verbose -resize $x $source $dest"
    convert -verbose -resize $x $source $dest

  done
done


for i in $sizes
do
  for x in $i/*
  do
    file=`basename $x`
    if ! [ -f "full/$file" ]; then
      echo "Warning: full/$file does not exist, but $x exists."
    fi

    id=`identify "$x"` || exit 1
    if [ "$id" = "" ] ; then
      echo "ERROR: Cannot dermine format and geometry for image: \"$x\"."
      continue
    fi
    g=`echo $id | awk '{print $3}'` || exit 1
    if [ "$g" = "" ] ; then
      echo "ERROR: Cannot dermine geometry for image: \"$x\"."
      continue
    fi

    # Geometry can be 563x144+0+0 or 75x98
    # we need to get rid of the plus (+) and the x characters:
    w=`echo $g | sed 's/[^0-9]/ /g' | awk '{print $1}'` || exit 1
    if [ "$w" = "" ] ; then
      echo "ERROR: Cannot dermine width for image: \"$x\"."
      continue
    fi
    h=`echo $g | sed 's/[^0-9]/ /g' | awk '{print $2}'` || exit 1
    if [ "$h" = "" ] ; then
      echo "ERROR: Cannot dermine height for image: \"$x\"."
      continue
    fi

    if ! [ "${h}x${w}" = "$i" ]; then
      echo "Warning: $x is not $i, but ${h}x${w}!"
    fi

  done
done

file="../icons.qrc"
rm -rf "$file"
echo "<RCC>" >>$file
echo "<qresource prefix=\"/\">" >>$file

for i in full $sizes
do
  for x in $i/*
  do
    f=`basename $x`
    echo "	<file>icons/$i/$f</file>" >>$file
  done
done

echo "</qresource>" >>$file
echo "</RCC>" >>$file

