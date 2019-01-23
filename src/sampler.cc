/* Continuous Time Bayesian Network Reasoning and Learning Engine
 * Copyright (C) 2009 The Regents of the University of California
 *
 * see docs/AUTHORS for contributor list
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "sampler.h"

namespace ctbn {

using namespace std;

Sampler::~Sampler() { 
}

void Sampler::SetMethod(const VarSample *m) {
	method = m;
}

void Sampler::SetTrajectory(const Trajectory *traj) {
	evid = traj;
	begintime = evid->TimeBegin();
	endtime = evid->TimeEnd();
}

} // end of ctbn namespace
