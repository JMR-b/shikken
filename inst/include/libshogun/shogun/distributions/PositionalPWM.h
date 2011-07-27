/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2011 Soeren Sonnenburg
 * Copyright (C) 2011 Berlin Institute of Technology and Max-Planck-Society
 */

#ifndef _POSITIONAL_PWM_H__
#define _POSITIONAL_PWM_H__

#include <shogun/distributions/Distribution.h>
#include <shogun/features/DotFeatures.h>
#include <shogun/lib/common.h>
#include <shogun/lib/Mathematics.h>

namespace shogun
{
class CPositionalPWM : public CDistribution
{
	public:
		/** default constructor */
		CPositionalPWM();

		virtual ~CPositionalPWM();

		/** learn distribution
		 *
		 * @param data training data
		 *
		 * @return whether training was successful
		 */
		virtual bool train(CFeatures* data=NULL);

		/** get number of parameters in model
		 *
		 * @return number of parameters in model
		 */
		virtual int32_t get_num_model_parameters();

		/** get model parameter (logarithmic)
		 *
		 * @return model parameter (logarithmic) if num_param < m_dim returns
		 * an element from the mean, else return an element from the covariance
		 */
		virtual float64_t get_log_model_parameter(int32_t num_param);

		/** get partial derivative of likelihood function (logarithmic)
		 *
		 * @param num_param derivative against which param
		 * @param num_example which example
		 * @return derivative of likelihood (logarithmic)
		 */
		virtual float64_t get_log_derivative(
			int32_t num_param, int32_t num_example);

		/** compute log likelihood for example
		 *
		 * abstract base method
		 *
		 * @param num_example which example
		 * @return log likelihood for example
		 */
		virtual float64_t get_log_likelihood_example(int32_t num_example);

		float64_t get_log_likelihood_window(uint8_t* window, int32_t len, float64_t pos);

		/** get sigma
		 *
		 * @param sigma
		 */
		virtual inline float64_t get_sigma()
		{
			return m_sigma;
		}

		/** set sigma
		 *
		 * @param sigma new sigma
		 */
		virtual inline void set_sigma(float64_t sigma)
		{
			m_sigma=sigma;
		}

		/** get mean
		 *
		 * @param mean
		 */
		virtual inline float64_t get_mean()
		{
			return m_mean;
		}

		/** set mean
		 *
		 * @param mean new mean
		 */
		virtual inline void set_mean(float64_t mean)
		{
			m_mean=mean;
		}

		/** set pwm
		 *
		 * @param pwm new pwm (values must be in logspace)
		 */
		virtual inline void set_pwm(SGMatrix<float64_t> pwm)
		{
			m_pwm=pwm.matrix;
			m_pwm_rows=pwm.num_rows;
			m_pwm_cols=pwm.num_cols;
		}

		/** get pwm
		 *
		 * @return current pwm
		 */
		virtual inline SGMatrix<float64_t> get_pwm()
		{
			return SGMatrix<float64_t>(m_pwm,m_pwm_rows,m_pwm_cols);
		}

		/** get w
		 *
		 * @return current w
		 */
		virtual inline SGMatrix<float64_t> get_w()
		{
			return SGMatrix<float64_t>(m_w,m_w_rows,m_w_cols);

		}

		void compute_scoring(float64_t** poim, int32_t* poim_len, int32_t max_degree);

		void compute_w(int32_t num_pos);

		/** @return object name */
		inline virtual const char* get_name() const { return "PositionalPWM"; }

	private:
		/** Initialize parameters for serialization */
		void register_params();

	protected:
		int32_t m_pwm_rows;
		int32_t m_pwm_cols;
		float64_t* m_pwm;

		float64_t m_sigma;
		float64_t m_mean;

		int32_t m_w_rows;
		int32_t m_w_cols;
		float64_t* m_w;

};
}
#endif //_POSITIONAL_PWM_H__
