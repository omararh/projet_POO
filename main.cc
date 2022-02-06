#include "projets.hh"

int main() {
	auto em1(etudiantm1::fabrique("NOM_M1_1", std::vector<langage>{langage::cplusplus, langage::java}));
	std::cout << (*em1) << "\n";
	auto em2(etudiantm2::fabrique("NOM_M2_1", 23));
	if (!em2) {
		std::cout << "em2 non créé (normal !)\n";
		em2=etudiantm2::fabrique("NOM_M2_1", 8);
	}
	std::cout << (*em2) << "\n";
	auto em3(etudiantm2::fabrique("NOM_M2_2", 10));
	std::cout << (*em3) << "\n";
	auto em4(etudiantm2::fabrique("NOM_M1_1", 10));
	universite u;
	u.ajout(em1);
	try {
		u.ajout(em4);
	}
	catch (exceptionuniversite const & e) {
		std::cout << "Erreur d'ajout (normal) " << e.what() << "\n";
	}
	auto prj1(std::make_shared<projet>());
	auto prj2(std::make_shared<projet>());
	u.ajout(prj1);
	u.ajout(prj2);
	prj1->ajout(em1);
	prj1->ajout(em2); // mais il ne fait pas partie de l'université
	try {
		prj1->ajout(em4);
	}
	catch (exceptionuniversite const & e) {
		std::cout << "Erreur d'ajout (normal) " << e.what() << "\n";
	}
	prj2->ajout(em1);
	prj2->ajout(em2);
	auto erreurs(u.verifierprojets());
	std::cout << "Verifierprojets (personnes dans un projet sans être dans l'université) : ";
	for (auto i : erreurs)
		std::cout << (*i) << " ";
	std::cout << "\n";
	return 0;
}
