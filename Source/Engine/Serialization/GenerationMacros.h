#pragma once

/**
 * @def REFLECT_CLASS(...)
 * @brief Macro to reflect a class ENGINE_API. This macro should be placed at the top
 * of the class ENGINE_API. It just a token for codegen tool.
 */
#define REFLECT_CLASS(...)

 /**
  * @def REFLECT_MEMBER(...)
  * @brief Macro to reflect a member variable. This macro should be placed
  * above the member variable. It just a token for codegen tool.
  */
#define REFLECT_MEMBER(...)

  /**
   * @def REFLECT_FUNC(...)
   * @brief Macro to reflect a member function. This macro should be placed
   * above the member function. It just a token for codegen tool.
   */
#define REFLECT_FUNC(...)

   /**
	* @def REFLECT_ENUM(...)
	* @brief Macro to reflect an enum. This macro should be placed at the top
	* of the enum. It just a token for codegen tool.
	*/
#define REFLECT_ENUM(...)

	/**
	 * @def GENERATE_MANIFEST
	 * @brief This macro will add the class ENGINE_API to the reflection manifest. This should
	 * be the first thing in the class ENGINE_API. It will add a couple friend class declarations
	 * in order to allow the serialization system to access the private members of the
	 * class ENGINE_API.
	 */
#define GENERATE_MANIFEST
