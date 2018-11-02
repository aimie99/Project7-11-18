#include "network.h"
#include "random.h"
#include <vector>
#include <map>
#include <random> 
#include <algorithm> 
#include <iostream>

void Network::resize(const size_t& n) {
	values.resize(n);	
	RNG.normal(values);
	
}
	
bool Network::add_link(const size_t& a, const size_t& b)
{	
	size_t nombre_de_nodes = values.size();
	//verifier que le lien n'existe pas déjà:
	auto iterator = links.equal_range(a);			
	bool existe_pas_deja(true);
	
	for (auto i = iterator.first; i != iterator.second && existe_pas_deja; ++i)
	{
		if (i->second == b)
		{
			existe_pas_deja = false;
		}
	}
	
	if (a < nombre_de_nodes and b < nombre_de_nodes and a != b and existe_pas_deja)
	{	
		links.insert(std::pair<size_t,size_t>(a, b)); 
		links.insert(std::pair<size_t,size_t>(b, a)); 
		return true;
	}		

	return false;
}


//on crée des connections au hasard parmi les nodes, chaque noeud n va etre lié à mean_deg autres nodes 
size_t Network::random_connect(const double& mean_deg)
{
	links.clear();
	int degree(0), compteur_de_liens(0);
	std::vector<int> valeurs_noeud_connecte;
	valeurs_noeud_connecte.push_back(0);
	
	for (size_t i(0); i < values.size(); ++i) // i c'est le node pour lequel je genere un degre et auquel nous connectons degre autres noeuds choisis aleatoirement
	{
		degree = RNG.poisson(mean_deg);
		for (int j(0); j<degree; ++j) 
		{ 
			do
			{
				RNG.uniform_int(valeurs_noeud_connecte, 0, values.size() - 1);
			}
			while (add_link(i, valeurs_noeud_connecte[0]) != true);
			++compteur_de_liens;
		}
	}
	return (size_t) compteur_de_liens;
}

size_t Network::set_values(const std::vector<double>& new_node_values) {

	size_t end = (size_t) std::min(new_node_values.size(), this->size()); 
	for (size_t i(0); i < end; ++i)
		values[i] = new_node_values[i];
	return end;
}

size_t Network::size() const
{
	return values.size();
}


size_t Network::degree(const size_t &_numero_node) const {
	auto iterator = links.equal_range(_numero_node);
	size_t compteur(0);
	
	for (auto arbitraire =  iterator.first; arbitraire != iterator.second; ++arbitraire)
	{
		++compteur;
	}
	
	return compteur;	
}


double Network::value(const size_t &_n) const {
	if (_n >= values.size())
	{
		std::cerr << "ce noeud n'existe pas" << std::endl;
		return 0;
	}
	
	return values[_n];
}

std::vector<double> Network::sorted_values() const {  
	std::vector<double> valeurs_rangees(values);
	std::sort(valeurs_rangees.begin(), valeurs_rangees.end());
	std::reverse(valeurs_rangees.begin(),valeurs_rangees.end());
	return valeurs_rangees;
}
	
	
std::vector<size_t> Network::neighbors(const size_t& _n) const {
	std::vector<size_t> stockage(0);
	
	if (!links.empty()) 
	{
		auto iterator = links.equal_range(_n);
		for (auto i = iterator.first; i != iterator.second; ++i) 
		{
			stockage.push_back(i->second); 
		}
	}
	
	return stockage;
}























