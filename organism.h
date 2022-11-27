#ifndef ORGANISM_H
#define ORGANISM_H

#include <optional>
#include <cstdint>
#include <tuple>

constexpr bool is_movable(bool eats_m, bool eats_p);

constexpr bool same_species(typename species_t, bool eats_m, bool eats_p);

template <typename species_t, bool can_eat_meat, bool can_eat_plants>
class Organism;

template <typename species_t>
using Carnivore = Organism<species_t, true, false>;

template <typename species_t>
using Omnivore = Organism<species_t, true, true>;

template <typename species_t>
using Herbivore = Organism<species_t, false, true>; 

template <typename species_t>
using Plant = Organism<species_t, false, false>;

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        bool sp2_eats_m, bool sp2_eats_p>
requires(isMovable(sp1_eats_m, sp1_eats_p) || isMovable(sp2_eats_m, sp2_eats_p))
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
          Organism<species_t, sp2_eats_m, sp2_eats_p>,
          std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2);

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1, Args... args);

///-----------------------------------------------------------------------------------------------

template <typename species_t, bool can_eat_meat, bool can_eat_plants>
class Organism {
public:
  constexpr uint64_t get_vitality() const { return vitality; }

  constexpr species_t const &get_species() const { 
     return species; }

  constexpr Organism(species_t const &species, uint64_t vitality) :
  species(species), vitality(vitality) { }

  constexpr bool is_dead() const { return vitality == 0; }

private: 
  species_t const &species;
  uint64_t vitality;
};

/// TODO: encounter
template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        bool sp2_eats_m, bool sp2_eats_p>
// 2. Nie jest możliwe spotkanie dwóch roślin,
requires(is_movable(sp1_eats_m, sp1_eats_p) || is_movable(sp2_eats_m, sp2_eats_p))
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
        Organism<species_t, sp2_eats_m, sp2_eats_p>,
        std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2) {
    // 1. Możliwe jest spotkanie jedynie organizmów, których typ gatunku (species_t) jest taki sam
    static_assert(typeof(organism1.get_species() == typeof(organism2.get_species(),
            "Species type must be the same in order for organisms to meet.");

    // 3. Spotkanie, w którym jedna ze stron jest martwa, nie powoduje skutków.
    if ((organism1.get_vitality() == 0) || (organism2.get_Vitality() == 0)) {
        return {Organism(organism1.get_species(), organism1.get_vitality()),
                Organism(organism2.get_species(), organism2.get_vitality())};
    }

    // 4. Spotkanie dwóch zwierząt tego samego gatunku prowadzi do godów.
    if (same_species(organism1,get_species(), sp1_eats_m, sp1_eats_p,
                     organism2.get_species(), sp2_eats_m, sp2_eats_p))) {
        auto species = organism1.get_species();
        int vitality1 = organism1.get_vitality();
        int vitality2 = organism2.get_vitality();

        return {Organism(organism1.get_species(), vitality1,
                Organism(organism2.get_species(), vitality2),
                Organism(species, ((vitality1 + vitality2) / 2))};
    }

    // 5. Spotkanie organizmów, które nie potrafią się zjadać, nie przynosi efektów.

    // 6. Spotkanie dwóch zwierząt, które potrafią się nawzajem zjadać, prowadzi do walki

    // 7. Spotkanie roślinożercy lub wszystkożercy z rośliną skutkuje tym, że roślina zostaje zjedzona.

    // 8. Przy spotkaniu, w którym zdolność do konsumpcji zachodzi tylko w jedną stronę, rozpatrujemy dwa przypadki.
    // Jeśli „potencjalnie zjadany" ma witalność nie mniejszą niż „potencjalnie zjadający", to nic się nie dzieje

    // W przeciwnym przypadku zjadający dodaje do swojej witalności połowę (zaokrągloną w dół) witalności zjedzonego, a zjedzony ginie.


    return {organism1, organism2, {}};
}

/// TODO: encounter_series
template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1, Args... args) {
    return organism1;
}

constexpr bool is_movable(bool eats_m, bool eats_p) { return eats_m || eats_p; }

constexpr bool same_species(typename sp1_species, bool sp1_eats_m, bool sp1_eats_p,
                            typename sp2_species, bool sp2_eats_m, bool sp2_eats_p) {
    return ((sp1_species == sp2_species) && (sp1_eats_m == sp2_eats_m)
    && (sp1_eats_p == sp2_eats_p));
}

// Carnivore – mięsożerca,
// Omnivore – wszystkożerca,
// Herbivore – roślinożerca,
// Plant – roślina, nie potrafi jeść ani mięsa, ani roślin.

#endif