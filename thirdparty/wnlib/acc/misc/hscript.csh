#!/bin/csh -f

set nonomatch

foreach hfile (*.h)
  if ( ! -e ../h/$hfile ) then
    ln -s ../misc/$hfile ../h
  endif
end

