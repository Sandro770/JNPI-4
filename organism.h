#ifndef ORGANISM_H
#define ORGANISM_H

template <typename species_t, bool can_eat_meat, bool can_eat_plants>
class Organism {

  uint64_t get_vitality();

  const species_t &get_species();

  Organism(species_t const &species, uint64_t vitality);
};

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, bool sp2_eats_m,
          bool sp2_eats_p>
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
                     Organism<species_t, sp2_eats_m, sp2_eats_p>,
                     std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2);

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
          typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
                 Args... args)

// Carnivore – mięsożerca,
// Omnivore – wszystkożerca,
// Herbivore – roślinożerca,
// Plant – roślina, nie potrafi jeść ani mięsa, ani roślin.

#endif