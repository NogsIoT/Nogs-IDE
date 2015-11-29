--[[
 *  CN Timer library
 *
 *  Copyright (C) Nogs GmbH, Andre Riesberg
 *  
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, 
 *  write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
--]]

-----------------------------------------------------------------------------
-- Author: Andre Riesberg
-- V2.1.1 Frontier version!
-- V2.1.2 timers is now a singlton
--
-- Timers module with function and coroutine support
--
-- Sample for a timer with call back function
-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
--
-- timer1 = timers.add(
--     1.0,                                  -- Cycle time 1 second
--     function()                            -- The function to call
--         print('timer01',os.timer())
--     end,
--     'timer1'                              -- Optinal timer name
-- )
--
--
-- Sample for a timer with a coroutine
-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
--
-- timer2 = timers.add(
--     1.0,                                  -- Cycle time 1 second
--     coroutine.create(                     -- Create the coroutine
--         function()                        -- The coroutine function
--             ...                           -- Init here
--              while true do
--                 print('timer2',os.timer())
--                 if ... then break end     -- Condition for timer termination
--                 coroutine.yield()         -- Wait for next timer cycle
--             end
--             ...                           -- Cleanup here. After finish the
--                                           -- function the timer will destroyed
--       end
--     ),
--     'timer2'                              -- Optinal timer name
-- )
--
-- For an error do a return nil, 'error test'
--
--
-----------------------------------------------------------------------------
if not timers then
  timers = {}
  timers.list = {} -- Hold the list of all timers closures
end
-----------------------------------------------------------------------------
--- Get the next timer id
-- Start with 1 and increase monotonic
-- @return timer id
-----------------------------------------------------------------------------
local function getNextId()
  if not timers.id then
    timers.id = 1
  else
    timers.id = timers.id + 1
  end
  return timers.id
end
-----------------------------------------------------------------------------
--- Add a new timer
-- @param intervalTime Called every interval time in seconds
-- @param worker The timer worker, could be a function or a coroutine
-- On sucresss:
-- @return timer id
-- On fail:
-- @return nil, message
-----------------------------------------------------------------------------
timers.add = function(intervalTime, worker, name)
  local timerId = getNextId() -- The id of the timer
  local timerElapsedTime = 0  -- Ellapsed time of a timer cycle
  local timerTotalTime = 0    -- Total time since timer start

  if type(worker) == 'function' then
    -- Worker is a function
    timers.list[timerId] = function(elapsedTime)
      timerElapsedTime = timerElapsedTime + elapsedTime
      if timerElapsedTime >= intervalTime then
        timerTotalTime = timerTotalTime + elapsedTime
        timerElapsedTime = timerElapsedTime - intervalTime
        worker(timerId, timerTotalTime) -- Call the worker function with timerId and total time
      end
    end
    return timerId
  end

  if type(worker) == 'thread' then
    -- Worker is a thread
    timers.list[timerId] = function(elapsedTime)
      if coroutine.status(worker) == 'suspended' then
        timerElapsedTime = timerElapsedTime + elapsedTime
        if timerElapsedTime >= intervalTime then
          timerTotalTime = timerTotalTime + elapsedTime
          timerElapsedTime = timerElapsedTime - intervalTime
          local ok, newIntervalTimeOrError = coroutine.resume(worker, timerId, timerTotalTime) -- Resume the coroutine with timerId and total time
          if not ok then
            -- An error condition kills the timer
            timers.list[timerId] = nil
            assert(nil, (name or '')..' '..newIntervalTimeOrError)
          end
          if newIntervalTimeOrError then
            -- Change the intervall time
            intervalTime = newIntervalTimeOrError
          end
        end
      end
    end
    return timerId
  end

  return nil, '\'worker\' must be a function or a thread'
end
-----------------------------------------------------------------------------
--- Delete a timer
-- @param timerId The timer id to delete
-----------------------------------------------------------------------------
timers.delete = function(timerId)
  timers.list[timerId] = nil
end
-----------------------------------------------------------------------------
--- Register timer callback function
-----------------------------------------------------------------------------
cn.onTickCallbacks[#cn.onTickCallbacks + 1] = function(elapsedTime)
  for _, timer in pairs(timers.list) do timer(elapsedTime) end
end
-----------------------------------------------------------------------------
--
-----------------------------------------------------------------------------
return timers
-----------------------------------------------------------------------------
--
-----------------------------------------------------------------------------
