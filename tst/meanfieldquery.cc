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
#include "markovdyn.h"
#include "streamextra.h"
#include "context.h"
#include "multirv.h"
#include "meanfieldinf.h"
#include "params.h"
#include "bn.h"
#include "ctbndyn.h"
#include "em.h"
#include "utils.h"
#include <iostream>
#include "ensurectbn.h"

using namespace std;
using namespace ctbn;

// this file reads in the input model & evidence & true query results from "queryinput.data"
// it calculate the queries using impotrance sampling and compare the results with the true ones
// if the error is within the threshold, this unittest successes, o.w.error message will pop out

namespace {
	int testing_result = 0;
	double kEpsilon = 0.2;
}

#define EXPECT(cond) do { \
        if (!(cond)) {                                                  \
			std::cerr << "Test failure at " << __FILE__ << ":" << __LINE__ \
                      << " in " << __func__ \
                      << ", failed condition is " #cond << std::endl; \
            testing_result = 1; \
        } \
    } while (0)

int main(int argc, char **argv)
{
	const char *input_file = "queryinput.data";
	addnan(cout);
	InitParams(argc, argv);
	double epsparam = ParamDouble("EPS", 1e-5);
 
	ifstream fin(input_file);
	Markov m;
	try {
		m.Load(fin);
	} catch (const serial::streamexception &e) {
		cerr << e.what() << endl;
		exit(-1);
	}
	const CTBNDyn *ctbndyn = dynamic_cast<const CTBNDyn *>(m.GetDynamics());
	Trajectory evid;
	evid.Load(fin);

	MeanFieldInf inf;
	inf.SetProcess(&m);
	inf.SetTrajectory(&evid);
	inf.SetEPS(epsparam);

	while(1) {
		int qtype;
		fin >> qtype;
		if (qtype==-1 || fin.eof()) break;
		int var, val;
		double t1;
		fin >> var >> val;
		double compans;
		Instantiation x(ctbndyn->Domain());
		x.SetVal(var,val);
		double eps = kEpsilon;
		switch(qtype) {
			case 0: // filter distribution at point
				fin >> t1;
				fin >> t1; // not implemented (remove answer)
				continue;
			case 1: // smooth distribution at point
				fin >> t1;
				compans = inf.Smooth(x,t1);
				eps *= 10;
			break;
			case 2: // expected time query
				{ QueryTime query(x); compans = inf.CalcQuery(query); }
			break;
			case 3: // expected # trans query
				{ int val2; fin >> val2;
					QueryTransition query(x.KnownVars(),val,val2);
					compans=inf.CalcQuery(query); }
			break;
		}
		double ans;
		fin >> ans;
		cout << "ans = " << compans << "\texpected " << ans << endl;
		EXPECT(fabs(ans-compans)/ans < eps);
	}
	fin.close();

	if (testing_result == 0)
		cout << "PASS meanfieldquery unittest." << endl;
	return testing_result!=0;
}
