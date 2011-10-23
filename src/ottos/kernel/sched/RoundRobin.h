/* RoundRobin.h
 * 
 * Copyright (c) 2011 The ottos project.
 *
 * This work is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This work is distributed in the hope that it will be useful, but without
 * any warranty; without even the implied warranty of merchantability or
 * fitness for a particular purpose. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *
 *  Created on: Oct 23, 2011
 *      Author: Matthias Schmid <m.schmid@students.fhv.at>
 */

#ifndef ROUNDROBIN_H_
#define ROUNDROBIN_H_

#include "../pm/Process.h"

class RoundRobin {
  public:
    static const int kInvalidProcess = -1;
    /* constructor */
    RoundRobin();
    /* destructor */
    virtual ~RoundRobin();
    /*
     * determines the next proc to be executed in round robin style.
     *
     * @returns int the index of the next proc to be executed or -1 if no
     * proc is READY or no proc available
     */
    int next(Process** procs, int cur_proc, int nr_of_procs);
};

#endif /* ROUNDROBIN_H_ */
