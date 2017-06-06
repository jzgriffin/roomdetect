// model.h
// Jeremiah Griffin <jgrif007@ucr.edu>
// Lab Section: 23
// Assignment: Custom Project - Room Detector - Compute Application
//
// I acknowledge all content contained herein, excluding template or example
// code, is my own original work.

#ifndef CUSTOM_COMPUTE_MODEL_H
#define CUSTOM_COMPUTE_MODEL_H

#include <stddef.h>
#include <stdfix.h>

/// \brief Size of the feature vector for the classification model
#define MODEL_VECTOR_SIZE 3

/// \brief Maximum number of rooms to be trained on the model
#define MODEL_ROOMS_MAX 0x0F

/// \brief Contains the statistics regarding a given element of the feature
/// vector
typedef struct {
    size_t samples; //!< Number of samples acquired
    accum mean; //!< Mean sample measured
    accum var; //!< Variance of the sample set
    accum stdev; //!< Standard deviation of the sample set
} vector_t;

/// \brief Contains the feature vector statistics for a given room
typedef struct {
    size_t samples; //!< Number of samples acquired
    vector_t vectors[MODEL_VECTOR_SIZE]; //!< Statistics for feature vector
} room_t;

/// \brief Contains a trained classification model
typedef struct {
    room_t rooms[MODEL_ROOMS_MAX];
} model_t;

/// \brief Clears the statistics for a feature vector
/// \param v element to clear
void clear_vector(vector_t *v);

/// \brief Updates the statistics for a feature vector given a reading
/// \param v statistics to update
/// \param x reading to integrate
void update_vector(vector_t *v, accum x);

/// \brief Scores a reading against the statistics for a feature vector
/// \param v statistics to score against
/// \param x reading to score
/// \return z-score representing the difference of \p x from the trained data
accum score_vector(const vector_t *v, accum x);

/// \brief Clears the statistics for a room
/// \param r room to clear
void clear_room(room_t *r);

/// \brief Updates the statistics for a room given a reading vector
/// \param r room to update
/// \param x vector of readings to integrate
void update_room(room_t *r, const accum *x);

/// \brief Scores a reading vector against a room statistics vector
/// \param r room to score against
/// \param x vector of readings to score
/// \return z-score representing the sum of absolute z-scores for the room
accum score_room(const room_t *r, const accum *x);

/// \brief Clears the training data for a model
/// \param m model to clear
void clear_model(model_t *m);

/// \brief Classifies a reading vector to the closest trained room
/// \param m model to classify against
/// \param x vector of readings to classify
/// \return number of predicted room or \p 0xFF if no room was matched
size_t classify_model(const model_t *m, const accum *x);

#endif // CUSTOM_COMPUTE_MODEL_H
