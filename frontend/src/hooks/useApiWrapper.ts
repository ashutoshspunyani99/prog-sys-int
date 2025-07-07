import { AxiosResponse } from 'axios';
import { useSnackbar } from '../contexts/SnackbarProvider.tsx';
import { ApiResponse } from '../types/ApiResponse.ts';

export function useApiWrapper() {
    const snackbar = useSnackbar();

    return async function ApiWrapper<T>(
        fn: () => Promise<AxiosResponse<ApiResponse<T>>>,
        options?: {
            fallback?: AxiosResponse<ApiResponse<T>>;
            suppressErrors?: boolean;
            context?: string;
            notifySuccess?: boolean;
            notifyFailure?: boolean;
        }
    ): Promise<AxiosResponse<ApiResponse<T>> | undefined> {
        const {
            fallback,
            suppressErrors = false,
            context,
            notifySuccess = true,
            notifyFailure = true,
        } = options || {};

        try {
            const res = await fn();

            // ✅ Get success message from response if available
            const successMessage = res?.data?.message;
            if (notifySuccess && successMessage) {
                snackbar.showSnackbar(successMessage, 'success');
            }

            return res;
        } catch (err: any) {
            const failureMessage =
                err.response?.data?.message || err.message || 'Unknown error';

            if (!suppressErrors) {
                console.error(`❌ API Error${context ? ` in ${context}` : ''}:`, failureMessage);
            }

            if (notifyFailure) {
                snackbar.showSnackbar(failureMessage, 'error');
            }

            return fallback;
        }
    };
}
