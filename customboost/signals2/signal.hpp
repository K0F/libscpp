//  A thread-safe version of Boost.Signals.

// Copyright Frank Mori Hess 2007-2009
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef BOOST_SIGNALS2_SIGNAL_HPP
#define BOOST_SIGNALS2_SIGNAL_HPP

#include <algorithm>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/shared_ptr.hpp>
#include <../customboost/signals2/connection.hpp>
#include <../customboost/signals2/detail/unique_lock.hpp>
#include <../customboost/signals2/detail/replace_slot_function.hpp>
#include <../customboost/signals2/detail/result_type_wrapper.hpp>
#include <../customboost/signals2/detail/signals_common.hpp>
#include <../customboost/signals2/detail/signals_common_macros.hpp>
#include <../customboost/signals2/detail/slot_groups.hpp>
#include <../customboost/signals2/detail/slot_call_iterator.hpp>
#include <../customboost/signals2/optional_last_value.hpp>
#include <../customboost/signals2/mutex.hpp>
#include <../customboost/signals2/slot.hpp>
#include <boost/throw_exception.hpp>
#include <functional>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#include <boost/signals2/preprocessed_signal.hpp>
#else
#include <boost/signals2/variadic_signal.hpp>
#endif

#endif // BOOST_SIGNALS2_SIGNAL_HPP
