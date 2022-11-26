#ifndef ORGANISM_H
#define ORGANISM_H

#include <optional>
#include <cstdint>
#include <tuple>

constexpr bool isMovable(bool m, bool p);

template <typename species_t, bool can_eat_meat, bool can_eat_plants>
class Organism;

template <typename species_t>
using Carnivore = Organism<species_t, true, false>;

template <typename species_t> using Omnivore = Organism<species_t, true, true>;

template <typename species_t>
using Herbivore = Organism<species_t, false, true>; 

template <typename species_t> using Plant = Organism<species_t, false, false>;


template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, bool sp2_eats_m,
          bool sp2_eats_p>
requires(isMovable(sp1_eats_m, sp1_eats_p) ||
         isMovable(sp2_eats_m, sp2_eats_p)) constexpr std::
    tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
          Organism<species_t, sp2_eats_m, sp2_eats_p>,
          std::optional<Organism<species_t, sp1_eats_m,
                                 sp1_eats_p>>> encounter(Organism<species_t,
                                                                  sp1_eats_m,
                                                                  sp1_eats_p>
                                                             organism1,
                                                         Organism<species_t,
                                                                  sp2_eats_m,
                                                                  sp2_eats_p>
                                                             organism2);

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
          typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
                 Args... args);


template <typename species_t, bool can_eat_meat, bool can_eat_plants>
class Organism {
public:

  constexpr uint64_t get_vitality() const { return 0; }

  constexpr species_t const &get_species() const { 
     return species; }

  constexpr Organism(species_t const &species, uint64_t vitality) : species(species) { }

  constexpr bool is_dead() const { return true; }

private: 
  species_t const &species;
};

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, bool sp2_eats_m,
          bool sp2_eats_p>
requires(isMovable(sp1_eats_m, sp1_eats_p) ||
         isMovable(sp2_eats_m, sp2_eats_p)) constexpr std::
    tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
          Organism<species_t, sp2_eats_m, sp2_eats_p>,
          std::optional<Organism<species_t, sp1_eats_m,
                                 sp1_eats_p>>> encounter(Organism<species_t,
                                                                  sp1_eats_m,
                                                                  sp1_eats_p>
                                                             organism1,
                                                         Organism<species_t,
                                                                  sp2_eats_m,
                                                                  sp2_eats_p>
                                                             organism2) {
                                                                 return {organism1, organism2, {}};
                                                             }


template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
          typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
                 Args... args) {
                    return organism1;
                 }

constexpr bool isMovable(bool m, bool p) { return m || p; }


// Carnivore – mięsożerca,
// Omnivore – wszystkożerca,
// Herbivore – roślinożerca,
// Plant – roślina, nie potrafi jeść ani mięsa, ani roślin.

#endif