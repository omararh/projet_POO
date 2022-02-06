#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <list>

enum class langage {
	cplusplus,
	java,
	php,
	javascript
};
//  simplifie l'écriture de etudiantm1::sortieflux
std::ostream & operator<<(std::ostream & os, langage t);

class personne {
	public:
		enum class type {
			M1, M2, Enseignant
		};
		personne(std::string const & n):_nom(n) {}
		~personne() =default;
		virtual type typepers() const=0;
		virtual void sortieflux(std::ostream & os) const;
		virtual bool expert() const=0;
		// Non demandé mais nécessaire pour rechercher par nom dans universite::ajout
		std::string const & nom() const { return _nom; }
	private:
		std::string _nom;
};
std::ostream & operator<<(std::ostream & os, personne const & p);
//  simplifie l'écriture de personne::sortieflux
std::ostream & operator<<(std::ostream & os, personne::type t);

class etudiant: public personne {
	public:
		etudiant(std::string const & n):personne(n), _numero(++_compteur) {}
		void sortieflux(std::ostream & os) const override { personne::sortieflux(os); os << " Num: " << _numero; }
	private:
		unsigned int _numero;
		static unsigned int _compteur;
};

class etudiantm1: public etudiant {
	private:
		etudiantm1(std::string const & n, std::vector<langage> const & langs):etudiant(n), _langages(langs) {}
	public:
		static std::shared_ptr<etudiantm1> fabrique(std::string const & n, std::vector<langage> const & langs);
		type typepers() const override { return type::M1; }
		bool maitriselangage(langage l) const { return std::find(_langages.begin(), _langages.end(), l) != _langages.end(); }
		void sortieflux(std::ostream & os) const override;
		bool expert() const override { return _langages.size() >= 3; }
	private:
		std::vector<langage> _langages;
};

class etudiantm2: public etudiant {
	private:
		etudiantm2(std::string const & n, unsigned short niv):etudiant(n), _niveau(niv) {}
	public:
		static std::shared_ptr<etudiantm2> fabrique(std::string const & n, unsigned short niv);
		type typepers() const override { return type::M2; }
		void sortieflux(std::ostream & os) const override { etudiant::sortieflux(os); os << " Niv: " << _niveau; }
		bool expert() const override { return _niveau >= 7; }
	private:
		unsigned short _niveau;
};

class enseignant: public personne {
	private:
		// constructeur privé et fabrique non obligatoire, mais proposés pour cohérence avec les autres classes.
		enseignant(std::string const & n, std::string bureau):personne(n), _bureau(bureau) {}
	public:
		static std::shared_ptr<enseignant> fabrique(std::string const & n, std::string const & bureau);
		type typepers() const override { return type::Enseignant; }
		void sortieflux(std::ostream & os) const override { personne::sortieflux(os); os << "Bureau: " << _bureau; }
		bool expert() const override { return true; }
	private:
		std::string _bureau;
};

class exceptionuniversite: public std::exception {
	public:
		exceptionuniversite(std::string const & m):std::exception(), _message(m) {}
		const char * what() const noexcept override { return _message.c_str(); }
	private:
		std::string _message;
};

class projet;
class universite {
	public:
		universite() =default;
		void ajout(std::shared_ptr<personne> a);
		std::vector<std::shared_ptr<etudiantm1>> rechercherdevs(langage l) const;
		void saisiem2();
		void ajout(std::shared_ptr<projet> a) { _projets.push_back(a); }
		std::list<std::shared_ptr<personne>> verifierprojets() const;
	private:
		std::vector<std::shared_ptr<personne>> _pers;
		std::vector<std::shared_ptr<projet>> _projets;
};

class projet {
	public:
		projet() =default;
		void ajout(std::shared_ptr<personne> a);
		std::vector<std::shared_ptr<personne>> const & membres() const { return _membres; }
	private:
		std::vector<std::shared_ptr<personne>> _membres;
};
