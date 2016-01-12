#include "gene.hpp"

Gene::Gene(): into(0), out(0),
	weight(0), innovation(0), enabled(true){
		//Left blank intentionally
	}
Gene::Gene(const int32_t& into_, const int32_t& out_):
	into(into_), out(out_), weight(0),
	innovation(0), enabled(true){
}

bool Gene::save(ofstream& ofs) const {
		/*Flush once at the end of the
		stream to make use of buffering
		efficency while preserving parse
		efficency and readibility*/

		ofs << into 				<< "\n";
		ofs << out					<< "\n";
		ofs << weight				<< "\n";
		ofs << innovation		<< "\n";
		ofs << enabled			<< "\n";
		return true;
}

bool Gene::load(ifstream& ifs){
	ifs >> into >> out >> weight >> innovation >> enabled;
	return true;
}
