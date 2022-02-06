#include "projets.hh"

std::ostream & operator<<(std::ostream & os, langage t) {
	switch (t) {
		case langage::cplusplus: os << "C++"; break;
		case langage::java: os << "Java"; break;
		case langage::php: os << "PHP"; break;
		case langage::javascript: os << "JS"; break;
	}
	return os;
}

std::ostream & operator<<(std::ostream & os, personne const & p) {
	p.sortieflux(os);
	return os;
}

std::ostream & operator<<(std::ostream & os, personne::type t) {
	switch (t) {
		case personne::type::M1: os << "M1"; break;
		case personne::type::M2: os << "M2"; break;
		case personne::type::Enseignant: os << "Enseignant"; break;
	}
	return os;
}

void personne::sortieflux(std::ostream & os) const {
	os << typepers() << " Nom: " << _nom;
}

unsigned int etudiant::_compteur(0);

std::shared_ptr<etudiantm1> etudiantm1::fabrique(std::string const & n, std::vector<langage> const & langs) {
	if (langs.size() >= 2)
		return std::shared_ptr<etudiantm1>(new etudiantm1(n, langs));
	else
		return nullptr;
}

void etudiantm1::sortieflux(std::ostream & os) const {
	etudiant::sortieflux(os);
	for (auto i : _langages)
		os << i << " ";
}

std::shared_ptr<etudiantm2> etudiantm2::fabrique(std::string const & n, unsigned short niv) {
	if ((niv >= 1) && (niv <= 10))
		return std::shared_ptr<etudiantm2>(new etudiantm2(n, niv));
	else
		return nullptr;
}

std::shared_ptr<enseignant> enseignant::fabrique(std::string const & n, std::string const & bureau) {
	return std::shared_ptr<enseignant>(new enseignant(n, bureau));
}

void universite::ajout(std::shared_ptr<personne> a) {
	auto noma(a->nom());
	auto f(std::find_if(_pers.begin(), _pers.end(), [noma](auto i){ return i->nom() == noma; }));
	if (f == _pers.end())
		_pers.push_back(a);
	else
		throw exceptionuniversite("Nom en double : " + noma);
}

std::vector<std::shared_ptr<etudiantm1>> universite::rechercherdevs(langage l) const {
	std::vector<std::shared_ptr<etudiantm1>> result;
	for (auto i : _pers) {
		auto iem1(std::dynamic_pointer_cast<etudiantm1>(i));
		if (iem1 && iem1->maitriselangage(l))
			result.push_back(iem1);
	}
	return result;
}

void universite::saisiem2() {
	bool redo(true);
	while (redo) {
		std::string nom; unsigned short niv;
		std::cout << "Nom : "; std::cin >> nom;
		std::cout << "Niveau : "; std::cin >> niv;
		try {
			ajout(etudiantm2::fabrique(nom, niv));
			redo = false;
		}
		catch (exceptionuniversite const & ee) {
			std::cout << "Erreur : " << ee.what() << "\n";
		}
	}
}

std::list<std::shared_ptr<personne>> universite::verifierprojets() const {
	std::list<std::shared_ptr<personne>> resultat;
	for (auto prj : _projets) {
		auto const & membresprj(prj->membres());
		for (auto mbp : membresprj)
			if (std::find(_pers.begin(), _pers.end(), mbp) == _pers.end()) {
				if (std::find(resultat.begin(), resultat.end(), mbp) == resultat.end())
					resultat.push_back(mbp);
			}
	}
	return resultat;
}

void projet::ajout(std::shared_ptr<personne> a) {
	if (std::dynamic_pointer_cast<enseignant>(a)) {
		if (std::count_if(_membres.begin(), _membres.end(), [](auto i){ return std::dynamic_pointer_cast<enseignant>(i);}) >= 2)
			throw exceptionuniversite("Déjà 2 enseignants");
	}
	if (std::dynamic_pointer_cast<etudiantm2>(a)) {
		auto nbm1 = std::count_if(_membres.begin(), _membres.end(), [](auto i){ return std::dynamic_pointer_cast<etudiantm1>(i);});
		auto nbm2 = std::count_if(_membres.begin(), _membres.end(), [](auto i){ return std::dynamic_pointer_cast<etudiantm2>(i);});
		if (nbm2 >= nbm1)
			throw exceptionuniversite("Il faut plus de M1 que de M2");
	}
	_membres.push_back(a);
}

